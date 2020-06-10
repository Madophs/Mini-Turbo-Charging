#include "GPRS.h"

GPRS::GPRS(unsigned int target_source, unsigned int data_consumed, std::string website)
	:Event("GPRS", "KB") {
	this->target_source_ = target_source;
	this->data_consumed_ = data_consumed;
	this->website_ = website;
}

/* Getters and Setters */

unsigned int GPRS::getTargetSource() const {
	return this->target_source_;
}

void GPRS::setTargetSource(unsigned int target_source) {
	this->target_source_ = target_source;
}

unsigned int GPRS::getDataConsumed() const {
	return this->data_consumed_;
}

void GPRS::setDataConsumed(unsigned int data_consumed) {
	this->data_consumed_ = data_consumed;
}

std::string GPRS::getWebsite() const {
	return this->website_;
}

void GPRS::setWebsite(std::string website) {
	this->website_ = website;
}

