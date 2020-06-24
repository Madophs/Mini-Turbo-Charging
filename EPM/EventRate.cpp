#include "EventRate.h"

EventRate::EventRate() {
	this->event_type_ = "unknown";
	this->unit_rate_ = 0;
	this->uom_ = "unknown";
}

bool EventRate::operator<(EventRate event_rate) {
	if (this->effective_date_.tm_year < event_rate.getEffectiveDate().tm_year)
		return true;

	if (this->effective_date_.tm_year == event_rate.getEffectiveDate().tm_year
		&& this->effective_date_.tm_mon < event_rate.getEffectiveDate().tm_mon)
		return true;

	if (this->effective_date_.tm_year == event_rate.getEffectiveDate().tm_year
		&& this->effective_date_.tm_mon == event_rate.getEffectiveDate().tm_mon
		&& this->effective_date_.tm_mday < event_rate.getEffectiveDate().tm_mday)
		return true;

	if (this->effective_date_.tm_year == event_rate.getEffectiveDate().tm_year
		&& this->effective_date_.tm_mon == event_rate.getEffectiveDate().tm_mon
		&& this->effective_date_.tm_mday == event_rate.getEffectiveDate().tm_mday)
		return this->event_type_ < event_rate.getEventType();
	return false;
}

bool EventRate::operator<=(EventRate event_rate) {
	if (*this < event_rate)
		return true;
	if (this->effective_date_.tm_year == event_rate.getEffectiveDate().tm_year
		&& this->effective_date_.tm_mon == event_rate.getEffectiveDate().tm_mon
		&& this->effective_date_.tm_mday == event_rate.getEffectiveDate().tm_mday
		&& this->event_type_ == event_rate.getEventType())
		return true;
	return false;
}

bool EventRate::operator>(EventRate event_rate) {
	return !(*this <= event_rate);
}

bool EventRate::operator>=(EventRate event_rate) {
	return !(*this < event_rate);
}

/* Getters and Setters */

std::string EventRate::getEventType() const {
	return this->event_type_;
}

void EventRate::setEventType(std::string event_type) {
	this->event_type_ = event_type;
}

tm EventRate::getEffectiveDate() const {
	return this->effective_date_;
}

void EventRate::setEffectiveDate(tm effective_date) {
	this->effective_date_ = effective_date;
}

std::string EventRate::getUOM() const {
	return this->uom_;
}

void EventRate::setUOM(std::string unit_of_measure) {
	this->uom_ = unit_of_measure;
}

int EventRate::getUnitAmount() const {
	return this->unit_amount_;
}

void EventRate::setUnitAmount(int unit_amount) {
	this->unit_amount_ = unit_amount;
}

int EventRate::getUnitRate() const {
	return this->unit_rate_;
}

void EventRate::setUnitRate(int unit_rate) {
	this->unit_rate_ = unit_rate;
}

std::ostream& operator << (std::ostream &out, EventRate event_rate) {
	int year, month, day;
	year = event_rate.getEffectiveDate().tm_year;
	month = event_rate.getEffectiveDate().tm_mon;
	day = event_rate.getEffectiveDate().tm_mday;
	out << "Event Type: " << event_rate.getEventType();
	out << "\nEffective Date: " << year << "-" << month << "-" << day;
	out << "\nUnit Of Measure: " << event_rate.getUOM();
	out << "\nUnit Amount: " << event_rate.getUnitAmount();
	out << "\nUnit Rate: " << event_rate.getUnitRate();
	return out;
}
