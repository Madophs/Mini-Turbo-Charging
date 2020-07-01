#include "PrepareEventObject.h"
#include "../GPRS.h"
#include "../SMS.h"
#include "../GSM.h"
#include "../Conf.h"
#include "../EventRate.h"
#include "../../DB/DB.h"
#include <memory>
#include <algorithm>
#define TRANSACTION_SIZE 10000

PrepareEvent::PrepareEvent() {
	this->db_management_ = nullptr;
	this->n_curr_rejected_events_ = 0;
	this->n_curr_rated_events_ = 0;
	this->n_rejected_events_ = 0;
	this->n_rated_events_ = 0;
}

PrepareEvent::PrepareEvent(DBManagement *db_management) {
	this->db_management_ = db_management;
	this->n_curr_rejected_events_ = 0;
	this->n_curr_rated_events_ = 0;
	this->n_rejected_events_ = 0;
	this->n_rated_events_ = 0;

	/* Create prepared events folder if not exists */
	system("mkdir -p events/.prepare_events/in_process events/.prepare_events/ready");

	/* Create and open transaction files */
	this->rated_event_writer_.open("events/.prepare_events/in_process/rated_events.csv", std::fstream::out | std::fstream::ate | std::fstream::trunc);
	this->rejected_event_writer_.open("events/.prepare_events/in_process/rejected_events.csv", std::fstream::out | std::fstream::ate | std::fstream::trunc);
}

void PrepareEvent::setDBManagement(DBManagement *db_management) {
	this->db_management_ = db_management;
}

int PrepareEvent::open(void *) {
	activate(THR_NEW_LWP, 1);
	return 0;
}

int PrepareEvent::svc() {
	if (!db_management_)
		return 1;
	ACE_DEBUG((LM_INFO, "(%t) Prepare Event Thread\n"));
	while(true) {
		prepareEvents();
		std::cout << n_rejected_events_ << std::endl;
		if ((n_rated_events_ + n_rejected_events_) % 10000 == 0 && false)
			std::cout << (n_rated_events_ + n_rejected_events_) << std::endl;
	}
	return 0;
}

int PrepareEvent::close(u_long) {
	ACE_DEBUG((LM_INFO, "(%t) Closing Prepare Event thread\n"));
	return 0;
}

void PrepareEvent::prepareEvents() {
	/* if no events to process, take a nap */
	if (event_queue.empty()) {
		/* Enqueue remaining events in transaction files */
		processRatedEventFile();
		processRejectedEventFile();

		/* Wait for more events */
		ACE_OS::sleep(EPM_Conf::getSleepTime());
	} else {
		processQueueEvents();
		/* If current number of events reach the transaction size, enqueue this events for insertion */
		if (n_curr_rated_events_ == TRANSACTION_SIZE) {
			processRatedEventFile();
			ACE_OS::sleep(1);
		}

		if (n_curr_rejected_events_ == TRANSACTION_SIZE) {
			processRejectedEventFile();
			ACE_OS::sleep(1);
		}
	}
}

void PrepareEvent::processQueueEvents() {
	/* receive event and pop it */
	Event *event = event_queue.front();
	event_queue.pop();

	/* Struct RatedEvent contains all information required to be inserted */
	RatedEvent rated_event;

	/* process (rate) event and save the results in rated_event struct */
	processEvent(event,rated_event);

	/* SQL statement */
	std::string sql_stmt = generateSQLStmt(rated_event, event);

	if (rated_event.accepted) {
		/* if the rated event is accepted, write this record into rated_events.csv transaction file */
		rated_event_writer_.write(sql_stmt.c_str(), sql_stmt.length());
		rated_event_writer_.put('\n');

		/* Count how many rated events we have so far */
		++n_curr_rated_events_, ++n_rated_events_;
	} else {
		/* if the rated event is rejected, write this record into rejected_events.csv transaction file */
		rejected_event_writer_.write(sql_stmt.c_str(), sql_stmt.length());
		rejected_event_writer_.put('\n');

		/* Count how many rejected events we have so far */
		++n_curr_rejected_events_, ++n_rejected_events_;
	}

	/* free some memory (event no longer required) */
	delete event;
}

void PrepareEvent::processEvent(Event *&event, RatedEvent &rated_event) {
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
}

void PrepareEvent::processRatedEventFile() {
	if (n_curr_rated_events_) {
		rated_event_writer_.close();
		/* Reset current rated events */
		n_curr_rated_events_ = 0;
		/* Move transaction file to ready directory */
		system("mv events/.prepare_events/in_process/rated_events.csv events/.prepare_events/ready/");
		/* Enqueue the transaction file into DB Mangement queue */
		db_management_->putq(new ACE_Message_Block("rated_events.csv"));
		/* Create and open a new transaction file */
		this->rated_event_writer_.open("events/.prepare_events/in_process/rated_events.csv");
	}
}
void PrepareEvent::processRejectedEventFile() {
	if (n_curr_rejected_events_) {
		rejected_event_writer_.close();
		/* Reset current rejected events count*/
		n_curr_rejected_events_ = 0;
		/* Move transaction file to ready directory */
		system("mv events/.prepare_events/in_process/rejected_events.csv events/.prepare_events/ready/");
		/* Enqueue the transaction file into DB Mangement queue */
		db_management_->putq(new ACE_Message_Block("rejected_events.csv"));
		/* Create and open a new transaction file */
		this->rejected_event_writer_.open("events/.prepare_events/in_process/rejected_events.csv");
	}
}
void PrepareEvent::addToDBQueue(std::string &sql_stmt) {

	/* Create a buffer that is going to contain the sql stmt */
	char *buff_sql_stmt = new char[sql_stmt.length()+1];

	/* Copy the contents of the SQL statement to the buffer */
	strcpy(buff_sql_stmt, sql_stmt.c_str());

	/* Create a new message block containing the buffer */
	ACE_Message_Block *mb_sql_stmt = new ACE_Message_Block(buff_sql_stmt, sizeof(buff_sql_stmt));

	/* Add message block to DB queue */
	db_management_->putq(mb_sql_stmt);
}

std::string PrepareEvent::generateSQLStmt(RatedEvent &rated_event, Event *&event) {
	std::string sql_stmt = "";

	/* if event is accepted insert in rated_event table otherwise rejected_event table */
	if (rated_event.accepted) {
		sql_stmt = rated_event.event_type + "|" + std::to_string(rated_event.target_source) + "|" +
			event->getStartTimeString() + "|" + std::to_string(rated_event.event_unit_consumed) + "|" +
			std::to_string(rated_event.total_charge);
	} else {
		sql_stmt = rated_event.event_type + "|" + std::to_string(rated_event.target_source) + "|" +
			event->getStartTimeString() + "|0|" +
			rated_event.rejected_reason;
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
