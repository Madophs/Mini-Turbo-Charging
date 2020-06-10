#ifndef SMS_EVENT_H
#define SMS_EVENT_H

#include "Event.h"

/* 
 * SMS (Short Message Service)
 *
 */

class SMS : public Event {
	public:
		SMS(unsigned int sender = 0, unsigned int receiver = 0, size_t no_chars = 0);

		unsigned int getSender() const;
		void setSender(unsigned int sender);

		unsigned int getReceiver() const;
		void setReceiver(unsigned int receiver);

		size_t getNumberOfChars() const;
		void setNumberOfChars(size_t no_char);

	private:
		unsigned int sender_;
		unsigned int receiver_;
		size_t no_chars_;
};

#endif
