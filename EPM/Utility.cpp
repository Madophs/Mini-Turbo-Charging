#include "Utility.h"
#include <sstream>
#include <algorithm>
#define ARRAY_SIZE 6 
#define SUCCESS 0
#define FAIL 1

std::string valid_date_formats[] = {"d/m/y", "d/y/m", "m/d/y", "m/y/d", "y/m/d", "y/d/m"};
std::string valid_time_formats[] = {"h/m/s", "h/s/m", "s/m/h", "s/h/m", "m/s/h", "m/h/s"};

bool Utility::isNumber(std::string value) {
	for (char &character : value) {
		if(!std::isdigit(character)) {
			return false;
		}
	}
	return true;
}

int Utility::setDatetime(std::string datetime, tm &struct_datetime, std::string format, char date_separator, char time_separator) {
	std::stringstream ss_datetime(datetime), ss_format(format);
	
	std::string date_format, time_format;

	/* Get Date format */
	if (!getline(ss_format, date_format, ' ')) {
		std::cerr << "Error: not valid Datetime format" << std::endl;
		return FAIL;
	}

	/* Get Time format */
	if (!getline(ss_format, time_format, ' ')) {
		std::cerr << "Error: not valid Datetime format" << std::endl;
		return FAIL;
	}

	std::string date, time;

	/* Get Date */
	if (!getline(ss_datetime, date, ' ')) {
		std::cerr << "Error: not valid Datetime" << std::endl;
		return FAIL;
	}

	/* Get Time */
	if (!getline(ss_datetime, time, ' ')) {
		std::cerr << "Error: not valid Datetime" << std::endl;
		return FAIL;
	}

	if (Utility::setDate(date, struct_datetime, date_format, date_separator) != 0)
		return FAIL;
	if (Utility::setTime(time, struct_datetime, time_format, time_separator) != 0)
		return FAIL;
	return SUCCESS;
}

int Utility::setDate(std::string date, tm &datetime, std::string format, char separator) {

	/* Check its a valid format */
	if (std::find(valid_date_formats, valid_date_formats + ARRAY_SIZE, format) == std::end(valid_date_formats)) {
		std::cerr << "Valid date format: " << format << std::endl;
		return FAIL;
	}

	std::stringstream ss_date(date), ss_format(format);
	std::string date_attr_value, date_attr;
	
	while (getline(ss_format, date_attr, '/')) {
		getline(ss_date, date_attr_value, separator);
		if (date_attr == "y") {
			datetime.tm_year = std::atoi(date_attr_value.c_str());
		} else if (date_attr == "m") {
			datetime.tm_mon = std::stoi(date_attr_value.c_str());
		} else {
			datetime.tm_mday = std::atoi(date_attr_value.c_str());
		}
	}
	return SUCCESS;
}

int Utility::setTime(std::string time, tm &datetime, std::string format, char separator) {

	/* Check its a valid format */
	if (std::find(valid_time_formats, valid_time_formats + ARRAY_SIZE, format) == std::end(valid_time_formats)) {
		std::cerr << "Valid time format: " << format << std::endl;
		return FAIL;
	}

	std::stringstream ss_time(time), ss_format(format);
	std::string time_attr_value, time_attr;

	while (getline(ss_format, time_attr, '/')) {
		getline(ss_time, time_attr_value, separator);
		if (time_attr == "h") {
			datetime.tm_hour = std::atoi(time_attr_value.c_str());
		} else if (time_attr == "m") {
			datetime.tm_min = std::atoi(time_attr_value.c_str());
		} else {
			datetime.tm_sec = std::atoi(time_attr_value.c_str());
		}
	}
	return SUCCESS;
}
