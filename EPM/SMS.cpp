#include "SMS.h"

SMS::SMS(unsigned int sender, unsigned int receiver, size_t no_chars):Event("SMS", "CHARS") {
	this->sender_ = sender;
	this->receiver_ = receiver;
	this->no_chars_ = no_chars;
}

/* Getters and Setters */

unsigned int SMS::getSender() const {
	return this->sender_;
}

void SMS::setSender(unsigned int sender) {
	this->sender_ = sender;
}

unsigned int SMS::getReceiver() const {
	return this->receiver_;
}

size_t SMS::getNumberOfChars() const {
	return this->no_chars_;
}

void SMS::setNumberOfChars(size_t no_char) {
	this->no_chars_ = no_char;
}

