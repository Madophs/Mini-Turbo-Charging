#include "Event.h"
#include <cstring>
#include "Utility.h"

Event::Event() {
	this->event_type_ = "unknown";
	this->unit_type_ = "undefined";
	this->rejected_reason_ = "";
	this->target_source_ = 0;
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

std::string Event::getStartTimeString() {
	std::string start_time = std::to_string(start_time_.tm_year);
	start_time.push_back('/');
	start_time += std::to_string(start_time_.tm_mon);
	start_time.push_back('/');
	start_time += std::to_string(start_time_.tm_mday);
	start_time.push_back(' ');
	start_time += std::to_string(start_time_.tm_hour);
	start_time.push_back(':');
	start_time += std::to_string(start_time_.tm_min);
	start_time.push_back(':');
	start_time += std::to_string(start_time_.tm_sec);
	return start_time;	
}

void Event::setStartTime(tm new_start_time) {
	this->start_time_ = new_start_time;
}

int Event::setStartTime(std::string start_time) {
	return Utility::setDatetime(start_time, this->start_time_, "d/m/y h/m/s");
}

std::string Event::getRejectedReason() const {
	return this->rejected_reason_;
}

void Event::setRejectedReason(std::string rejected_reason) {
	this->rejected_reason_ = rejected_reason;
}

unsigned int Event::getTargetSource() const {
	return this->target_source_;
}

void Event::setTargetSource(unsigned int target_source) {
	this->target_source_ = target_source;
}
