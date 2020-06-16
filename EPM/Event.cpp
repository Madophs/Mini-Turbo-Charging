#include "Event.h"
#include <cstring>
#include "Utility.h"

Event::Event() {
	this->event_type_ = "unknown";
	this->unit_type_ = "undefined";
	memset((void *) &start_time_, 0, sizeof(start_time_));
}

Event::Event(std::string event_type, std::string unit_type) {
	this->event_type_ = event_type;
	this->unit_type_ = unit_type;
	memset((void *) &start_time_, 0, sizeof(start_time_));
}

/* Getters and Setters */

std::string Event::getEventType() const {
	return this->event_type_;
}

void Event::setEventType(std::string new_event_type) {
	this->event_type_ = new_event_type;
}

std::string Event::getUnitType() const {
	return this->unit_type_;
}

void Event::setUnitType(std::string new_unit_type) {
	this->unit_type_ = new_unit_type;
}

tm &Event::getStartTime() {
	return this->start_time_;
}

void Event::setStartTime(tm new_start_time) {
	this->start_time_ = new_start_time;
}

void Event::setStartTime(std::string start_time) {
	Utility::setDatetime(start_time, this->start_time_, "d/m/y h/m/s");
}
