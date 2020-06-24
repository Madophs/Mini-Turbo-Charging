#include "PrepareEventObject.h"
#include "../GPRS.h"
#include "../SMS.h"
#include "../GSM.h"
#include "../Conf.h"
#include "../EventRate.h"
#include "../../DB/DB.h"
#include <memory>
#include <algorithm>

PrepareEvent::PrepareEvent() {
	this->db_management_ = nullptr;
}

PrepareEvent::PrepareEvent(DBManagement *db_management) {
	this->db_management_ = db_management;
}

int PrepareEvent::open(void *) {
	ACE_DEBUG((LM_INFO, "(%t) Prepare event thread\n"));
	activate(THR_NEW_LWP, 1);
	return 0;
}

int PrepareEvent::svc() {
	if (!db_management_)
		return 0;
	int process_count = 0;
	ACE_DEBUG((LM_DEBUG, "(%t) Prepare Event Thread\n"));
	while(true) {

		/* if no events to process, take a nap */
		if (event_queue.empty()) {
			ACE_OS::sleep(1);
			continue;
		}
		process_queue_events();
		if (++process_count % 10000 == 0)
			std::cout << ++process_count << std::endl;
	}
	return 0;
}

int PrepareEvent::close(u_long) {
	ACE_DEBUG((LM_INFO, "(%t) Closing Prepare Event thread\n"));
	return 0;
}

void PrepareEvent::process_queue_events() {
	/* receive event and pop it */
	Event *event = event_queue.front();
	event_queue.pop();

	/* Struct RatedEvent contains all information required to be inserted */
	RatedEvent rated_event;

	process_event(event,rated_event);

	delete event;
}

void PrepareEvent::process_event(Event *event, RatedEvent &rated_event) {
	/* Initialize the rated event */
	rated_event.target_source = event->getTargetSource();
	rated_event.event_type = event->getEventType();
	rated_event.start_time = event->getStartTime();
	rated_event.event_unit_consumed = 0.0f;
	rated_event.total_charge = 0.0f;
	rated_event.accepted = event->getRejectedReason().empty();
	rated_event.rejected_reason = event->getRejectedReason();

	EventRate event_rate;

	/* Set attributes to be search */
	event_rate.setEventType(event->getEventType());
	event_rate.setEffectiveDate(event->getStartTime());

	if (event->getRejectedReason().empty() && !EventRateScheme::findEffectiveDate(event_rate)) {
		rated_event.accepted = false;
		rated_event.rejected_reason = "Effective date not found.";
	}

	if (event->getEventType() == "SMS") {
		SMS *sms_event = reinterpret_cast<SMS *>(event);
		rated_event.target_source = sms_event->getSender();
		if (rated_event.accepted) {
			rated_event.event_unit_consumed = ((float)sms_event->getNumberOfChars() / (float)event_rate.getUnitAmount());
			rated_event.total_charge = (rated_event.event_unit_consumed * event_rate.getUnitRate());
		}
	} else if (event->getEventType() == "GPRS") {
		GPRS *gprs_event = reinterpret_cast<GPRS *>(event);
		rated_event.target_source = gprs_event->getTargetSource();
		if (rated_event.accepted) {
			rated_event.event_unit_consumed = ((float)gprs_event->getDataConsumed() / (float)event_rate.getUnitAmount());
			rated_event.total_charge = (rated_event.event_unit_consumed * event_rate.getUnitRate());
		}
	} else if (event->getEventType() == "GSM") {
		GSM *gsm_event = reinterpret_cast<GSM *>(event);
		rated_event.target_source = gsm_event->getCaller();
		if (rated_event.accepted) {
			rated_event.event_unit_consumed = ((float)gsm_event->getCallDuration() / (float)event_rate.getUnitAmount());
			rated_event.total_charge = (rated_event.event_unit_consumed * event_rate.getUnitRate());
		}
	}
	if (db_management_ != nullptr) {
		/* SQL statement */
		std::string sql_stmt = generateSQLStmt(rated_event, event);

		/* add to DB queue */
		addToDBQueue(sql_stmt);
	}
}

void PrepareEvent::addToDBQueue(std::string &sql_stmt) {
	char *buffer = new char[sql_stmt.length()+1];
	strcpy(buffer, sql_stmt.c_str());
	ACE_Message_Block *mb_sql_stmt = new ACE_Message_Block(buffer, sizeof(buffer));
	db_management_->putq(mb_sql_stmt);
}

std::string PrepareEvent::generateSQLStmt(RatedEvent &rated_event, Event *&event) {
	std::string sql_stmt = "";
	/* if event is accepted insert in rated_event table otherwise rejected_event table */
	if (rated_event.accepted) {
		sql_stmt = sql_stmt + "INSERT INTO rated_event (event_type, target_source, event_start_time, event_unit_consumed, total_charge) " +
			"VALUES ('" + rated_event.event_type + "', " + std::to_string(rated_event.target_source) + ", '" +
			event->getStartTimeString() + "', " + std::to_string(rated_event.event_unit_consumed) + ", " +
			std::to_string(rated_event.total_charge) + ");";
	} else {
		sql_stmt = sql_stmt + "INSERT INTO rejected_event (event_type, target_source, event_start_time, event_unit_consumed, rejected_reason) " +
			"VALUES ('" + rated_event.event_type + "', " + std::to_string(rated_event.target_source) + ", '" +
			event->getStartTimeString() + "', 0, '" +
			rated_event.rejected_reason + "');";
	}
	return sql_stmt;
}

std::string PrepareEvent::rate_event_test(RatedEvent &rated_event) {
	std::string result = "";
	std::string start_time = std::to_string(rated_event.start_time.tm_year) +
		"-" + std::to_string(rated_event.start_time.tm_mon) + "-" + std::to_string(rated_event.start_time.tm_mday);
	result += rated_event.event_type + "," + std::to_string(rated_event.target_source) + "," + start_time;
	if (rated_event.accepted) {
		result += "," + std::to_string(rated_event.event_unit_consumed) + "," + std::to_string(rated_event.total_charge);
	} else {
		result += "," + rated_event.rejected_reason;
	}
	return result;
}
