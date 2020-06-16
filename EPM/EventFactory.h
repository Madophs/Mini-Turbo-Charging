#ifndef EVENT_FACTORY_H
#define EVENT_FACTORY_H

#include "SMS.h"
#include "GPRS.h"
#include "GSM.h"

class EventFactory {
	public:
		static Event * createEvent(std::string event_type, std::string unit_type);

		static SMS * createSMSEvent(unsigned int sender = 0,
									unsigned int receiver = 0,
									size_t no_chars = 0);

		static GPRS * createGPRSEvent(unsigned int target_source = 0,
									  unsigned int data_consumed = 0,
									  std::string website = "undefined");

		static GSM * createGSMEvent(unsigned int caller = 0,
									unsigned int callee = 0,
									unsigned int call_duration = 0);

		/*
		 * Create a event from unparse data (std::string)
		 * Format: EventType (GSM, SMS, GPRS), Target_source, Target, data_consumed, datetime
		 * Example: GPRS,2532,facebook.com,400, 30/05/2020 11:30:05
		 *
		 */

		static Event * createEventFromUnparsedData(std::string event_data);
	private:
};

#endif
