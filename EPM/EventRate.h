#ifndef EVENT_RATE_H
#define EVENT_RATE_H

#include "../DB/DB.h"
#include "Event.h"
#include <ctime>
#include <vector>

class EventRate {
	public:
		EventRate();

		/* Very useful operators to find effective date */
		bool operator < (EventRate event_rate);
		bool operator <= (EventRate event_rate);
		bool operator > (EventRate event_rate);
		bool operator >= (EventRate event_rate);
		
		std::string getEventType() const;
		void setEventType(std::string event_type);

		tm getEffectiveDate() const;
		void setEffectiveDate(tm effective_data);

		/* Unit of Measure */
		std::string getUOM() const;
		void setUOM(std::string uom);

		int getUnitAmount() const;
		void setUnitAmount(int unit_amount);

		int getUnitRate() const;
		void setUnitRate(int unit_rate);

		/* Friend function that prints EventRate atributes using cout */
		friend std::ostream& operator << (std::ostream &out, EventRate event_rate);
	private:
		std::string event_type_;
		tm effective_date_;
		/* Unit of Measure */
		std::string uom_;
		int unit_amount_;
		int unit_rate_;
};

/* 
 * EventRateScheme: Singleton Class
 *
 * Contains the EVENT_RATE's records
 *
 */

class EventRateScheme {
	public:
		EventRateScheme(const EventRateScheme&) = delete;
		static EventRateScheme &instance();
		
		/*
		 * If an effective date is found
		 * returns true and set event_rate_search to the respective event rate
		 * Otherwise return false
		 */
		static bool findEffectiveDate(EventRate &event_rate_search);
		static void updateScheme();
		static std::vector<EventRate> getRecords();
	private:
		EventRateScheme();

		/* Event rate records */
		std::vector<EventRate> event_rate_rec_;

		/* Internal functions */
		void iUpdateScheme();
};

#endif
