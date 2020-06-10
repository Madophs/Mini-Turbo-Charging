#include "GSM.h"

GSM::GSM(unsigned int caller, unsigned int callee, unsigned int call_duration)
	:Event("GSM", "SECONDS") {
		this->caller_ = caller;
		this->callee_ = callee;
		this->call_duration_ = call_duration;
	}

/* Getters and Setters */

unsigned int GSM::getCaller() const {
	return this->caller_;
}

void GSM::setCaller(unsigned int new_caller) {
	this->caller_ = new_caller;
}

unsigned int GSM::getCallDuration() const {
	return this->call_duration_;
}

void GSM::setCallDuration(unsigned int new_call_duration) {
	this->call_duration_ = new_call_duration;
}

unsigned int GSM::getCallee() const {
	return this->callee_;
}

void GSM::setCallee(unsigned int new_callee) {
	this->callee_ = new_callee;
}

