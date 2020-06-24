#ifndef EVENT_CLASS_H
#define EVENT_CLASS_H

#include <iostream>
#include <ctime>

class Event {
	public:

		Event();
		Event(std::string event_type, std::string unit_type);

		std::string getEventType() const;
		void setEventType(std::string event_type);

		/* Unit type = Unit of Measure */
		std::string getUnitType() const;
		void setUnitType(std::string unit_type);

		tm &getStartTime();
		std::string getStartTimeString();
		void setStartTime(tm start_time);
		int setStartTime(std::string start_time);

		std::string getRejectedReason() const;
		void setRejectedReason(std::string rejected_reason);

		virtual unsigned int getTargetSource() const;
		virtual void setTargetSource(unsigned int target_source);

	private:
		unsigned int target_source_;
	protected:

		std::string event_type_;
		std::string unit_type_;
		std::string rejected_reason_;

		/* struct tm holds the date and time */
		tm start_time_;
};

#endif
