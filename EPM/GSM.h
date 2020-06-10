#ifndef GSM_EVENT_H
#define GSM_EVENT_H

#include "Event.h"

/*
 * GSM (Global System for Mobile)
 *
 */

class GSM : public Event {
	public:
		GSM(unsigned int caller = 0, unsigned int callee = 0, unsigned int call_duration = 0);
		
		unsigned int getCaller() const;
		void setCaller(unsigned int new_caller);

		unsigned int getCallDuration() const;
		void setCallDuration(unsigned int new_call_duration);

		unsigned int getCallee() const;
		void setCallee(unsigned int callee);

	private:
		unsigned int caller_;
		unsigned int call_duration_;
		unsigned int callee_;
};

#endif
