#ifndef GPRS_EVENT_H
#define GPRS_EVENT_H

#include "Event.h"

/*
 * GPRS (General Package Radio Service)
 *
 */

class GPRS : public Event {
	public:
		GPRS(unsigned int target_source_ = 0, unsigned int data_consumed = 0, std::string website = "undefined");

		unsigned int getTargetSource() const;
		void setTargetSource(unsigned int target_source);

		unsigned int getDataConsumed() const;
		void setDataConsumed(unsigned int data_consumed);

		std::string getWebsite() const;
		void setWebsite(std::string website);

	private:
		unsigned int target_source_;
		unsigned int data_consumed_;
		std::string website_;
};

#endif
