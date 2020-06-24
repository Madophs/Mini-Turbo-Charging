#include "EventFactory.h"
#include <sstream>
#include "Utility.h"
#include <cstring>
#include <bitset>
#define MAX_LENGTH 50
#define MAX_INTERGER_LENGTH 10

Event * EventFactory::createEvent(std::string event_type, std::string unit_type) {
	return new Event(event_type, unit_type);
}

SMS * EventFactory::createSMSEvent(unsigned int sender, unsigned int receiver, size_t no_chars) {
	return new SMS(sender, receiver, no_chars);
}

GSM * EventFactory::createGSMEvent(unsigned int caller, unsigned int callee, unsigned int call_duration) {
	return new GSM(caller, callee, call_duration);
}

GPRS * EventFactory::createGPRSEvent(unsigned int target_source, unsigned int data_consumed, std::string website) {
	return new GPRS(target_source, data_consumed, website);
}

Event * EventFactory::createEventFromUnparsedData(std::string event_data) {
	Event *event = nullptr;

	parseEventData(event_data);
	setErrors();
	
	if (event_type_ == "SMS") {
		event = createSMSEvent(std::atoi(target_source_.c_str()), std::atoi(target_.c_str()), std::atoi(data_consumed_.c_str()));
		event->setRejectedReason(rejected_reason_);
	} else if (event_type_ == "GSM") {
		event = createGSMEvent(std::atoi(target_source_.c_str()), std::atoi(target_.c_str()), std::atoi(data_consumed_.c_str()));
		event->setRejectedReason(rejected_reason_);
	} else if (event_type_ == "GPRS") {
		event = createGPRSEvent(std::atoi(target_source_.c_str()), std::atoi(data_consumed_.c_str()), target_);
		event->setRejectedReason(rejected_reason_);
	} else {
		event = createEvent(event_type_, "unknown");
		event->setTargetSource(std::atoi(target_source_.c_str()));
	}
	event->setStartTime(start_time_);
	event->setRejectedReason(rejected_reason_);

	return event;
}

void EventFactory::parseEventData(std::string &event_data) {
	std::stringstream ss_event(event_data);	
	event_type_ = "", target_source_ = "0", start_time_str_ = "", data_consumed_ + "0", target_ = "0";
	memset((void *) &start_time_, 0, sizeof(start_time_));

	/*
	 * Bit operations
	 *
	 * 1  = 1 << 0 = Missing Data
	 * 2  = 1 << 1 = Unknown Event Type
	 * 4  = 1 << 2 = Invalid Target Source
	 * 8  = 1 << 3 = Invalid Target
	 * 16 = 1 << 4 = Data Consumed Invalid Value
	 * 32 = 1 << 5 = Invalid Start time
	 * 64 = 1 << 6 = Extra data
	 *
	 */

	/* This variable contains all error codes, it's use as a bitmask */
	this->errors = 0;

	/* Get attributes and trim whitespaces */

	/* Get event type */
	getline(ss_event, event_type_, ',');
	Utility::trim(event_type_);

	if (event_type_ != "GSM" && event_type_ != "SMS" && event_type_ != "GPRS") {
		/* Unknown event type */
		errors |= (1u << 1);
	}

	/* Get Target Source */
	if (!getline(ss_event, target_source_, ',')) {
		/* Missing data */
		errors |= (1u);
	} else {
		Utility::trim(target_source_);
		if (!Utility::isNumber(target_source_) || target_source_.length() > MAX_INTERGER_LENGTH) {
			/* Invalid Target Source */
			errors |= (1u << 2);
		}
	}

	/* Get Start time */
	if (!getline(ss_event, start_time_str_, ',')) {
		/* Missing data */
		errors |= (1u);
	} else {
		Utility::trim(start_time_str_);
		if (Utility::setDatetime(start_time_str_, start_time_, "d/m/y h/m/s") != 0) {
			/* Invalid datetime */
			errors |= (1u << 5);
		}
	}

	/* Get Data consumed */
	if (!getline(ss_event, data_consumed_, ',')) {
		/* Missing data */
		errors |= (1u);
	} else {
		Utility::trim(data_consumed_);
		if(!Utility::isNumber(data_consumed_) || data_consumed_.length() > MAX_INTERGER_LENGTH) {
			/* Invalid Target value */
			errors |= (1u << 4);
		}
	}

	/* Get target */
	if (!getline(ss_event, target_, ',')) {
		/* Missing data (Target)*/
		errors |= (1u);
	} else {
		Utility::trim(target_);
		if (event_type_ != "GPRS") {
			if(!Utility::isNumber(target_) || target_.length() > MAX_INTERGER_LENGTH) {
				/* Invalid target */
				errors |= (1u << 3);
			}
		}
	}
}

void EventFactory::setErrors() {
	rejected_reason_ = "";

	/* If no errors found just return */
	if (!errors) return;

	/*
	 * Bit operations
	 *
	 * 1  = 1 << 0 = Missing Data
	 * 2  = 1 << 1 = Unknown Event Type
	 * 4  = 1 << 2 = Invalid Target Source
	 * 8  = 1 << 3 = Invalid Target
	 * 16 = 1 << 4 = Data Consumed Invalid Value
	 * 32 = 1 << 5 = Invalid Start time
	 * 64 = 1 << 6 = Extra data (Too many arguments)
	 *
	 */

	if (errors & 1u) {
		/* if unknown event name is too large change it to "unknown" */
		rejected_reason_ = "Missing data.";
	} else {
		
		/* Too many invalid values */
		if(std::bitset<32>(errors).count() > 2) {
			rejected_reason_ = "Invalid values.";
			return;
		}

		/* if bits are set */
		if (errors & (1u << 1)) {
			/* if unknown event name is too large change it to "unknown" */
			if (event_type_.length() > MAX_LENGTH) {
				event_type_ = "unknown";
				rejected_reason_ = "Unknown event, ";
			} else {
				rejected_reason_ += "Unknown Event " + event_type_ + ", ";
			}
		}

		if (errors & (1u << 2)) {

			/* check if Target source value is too large */
			if (target_source_.length() > MAX_LENGTH) {
				rejected_reason_ += "Invalid target source, ";
			} else {
				rejected_reason_ += "Invalid target source " + target_source_ + ", ";
			}
			/* if Target source is not valid, then reset the value to 0. (Same with others) */
			target_source_ = "0";
		}

		if (errors & (1u << 3)) {
			
			/* Check if Target value is too large (too many chars) */
			if (target_.length() > MAX_LENGTH) {
				rejected_reason_ += "Invalid target, ";
			} else {
				rejected_reason_ += "Invalid target " + target_ + ", ";
			}

			/* Reset target value */
			target_ = "0";
		}

		if (errors & (1u << 4)) {
			if (data_consumed_.length() < MAX_LENGTH) {
				rejected_reason_ += "Invalid data consumed value: " + data_consumed_ + ", ";
			} else {
				rejected_reason_ += "Invalid data consumed value, ";
			}
			data_consumed_ = "0";
		}

		if (errors & (1u << 5)) {
			if (data_consumed_.length() < MAX_LENGTH) {
				rejected_reason_ += "Invalid Start Time: " + start_time_str_ + ", ";
			}
		}

		/* This is just for good formating. */
		if (errors < (1u << 6)) {
			rejected_reason_.pop_back(), rejected_reason_.pop_back();
			rejected_reason_.push_back('.');
		}
		if (errors & (1u << 6))
			rejected_reason_ += "Invalid Event format.";
	}

}

