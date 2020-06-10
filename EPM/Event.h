#ifndef EVENT_CLASS_H
#define EVENT_CLASS_H

#include <iostream>
#include <ctime>

class Event {
	public:

		Event();
		Event(std::string event_type, std::string unit_type);

		std::string getEventType() const;
		void setEventType(std::string new_event_type);

		std::string getUnitType() const;
		void setUnitType(std::string new_unit_type);

		tm &getStartTime();
		void setStartTime(tm new_start_time);

	protected:

		std::string event_type_;
		std::string unit_type_;

		/* struct tm holds the date and time */
		tm start_time_;
};

#endif