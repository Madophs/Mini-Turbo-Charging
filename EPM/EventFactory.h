#ifndef EVENT_FACTORY_H
#define EVENT_FACTORY_H

#include "SMS.h"
#include "GPRS.h"
#include "GSM.h"

/*
 * Create Events
 *
 */

class EventFactory {
	public:

		Event * createEvent(std::string event_type, std::string unit_type);

		SMS * createSMSEvent(unsigned int sender = 0,
									unsigned int receiver = 0,
									size_t no_chars = 0);

		GPRS * createGPRSEvent(unsigned int target_source = 0,
									  unsigned int data_consumed = 0,
									  std::string website = "undefined");

		GSM * createGSMEvent(unsigned int caller = 0,
									unsigned int callee = 0,
									unsigned int call_duration = 0);

		/*
		 * Create a event from unparse data (std::string)
		 * Format: EventType (GSM, SMS, GPRS), Target Source,Start Time, Data Consumed, Target
		 * Example: GPRS,2532,30/05/2020 11:30:05,400,facebook.com 
		 *
		 */

		Event * createEventFromUnparsedData(std::string event_data);

	private:
		std::string event_type_;
		std::string target_source_;
		std::string target_;
		std::string data_consumed_;
		std::string start_time_str_;
		std::string rejected_reason_;
		tm start_time_;
		unsigned int errors;

		void parseEventData(std::string &event_data);
		void setErrors();
};

#endif
