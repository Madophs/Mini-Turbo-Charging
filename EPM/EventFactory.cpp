#include "EventFactory.h"
#include <sstream>
#include "Utility.h"

	
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
	std::stringstream ss_event(event_data);	
	std::string event_type, target_source, start_time, data_consumed, target;
	Event *event = nullptr;

	/* Get attributes and trim whitespaces */
	getline(ss_event, event_type, ',');
	Utility::trim(event_type);
	getline(ss_event, target_source, ',');
	Utility::trim(target_source);
	getline(ss_event, target, ',');
	Utility::trim(target);
	getline(ss_event, data_consumed, ',');
	getline(ss_event, start_time, ',');
	Utility::trim(start_time);

	if (event_type == "SMS") {
		event = createSMSEvent(std::atoi(target_source.c_str()), std::atoi(target.c_str()), std::atoi(data_consumed.c_str()));
		event->setStartTime(start_time);
	} else if (event_type == "GSM") {
		event = createGSMEvent(std::atoi(target_source.c_str()), std::atoi(target.c_str()), std::atoi(data_consumed.c_str()));
		event->setStartTime(start_time);
	} else if (event_type == "GPRS") {
		event = createGPRSEvent(std::atoi(target_source.c_str()), std::atoi(data_consumed.c_str()), target);
		event->setStartTime(start_time);
	} else {
		event = createEvent(event_type, "unknown");
		event->setStartTime(start_time);
	}
	return event;
}
