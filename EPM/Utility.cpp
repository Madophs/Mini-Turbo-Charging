#include "Utility.h"
#include <sstream>
#include <algorithm>
#define ARRAY_SIZE 6 
#define SUCCESS 0
#define FAIL 1
#define INVALID_VALUE 2
#define OUT_OF_RANGE 3

const std::string valid_date_formats[] = {"d/m/y", "d/y/m", "m/d/y", "m/y/d", "y/m/d", "y/d/m"};
const std::string valid_time_formats[] = {"h/m/s", "h/s/m", "s/m/h", "s/h/m", "m/s/h", "m/h/s"};
const int month_days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

bool Utility::isNumber(std::string &value) {
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

	/* ret_val constains the value returned by the function, if val !=0 means something went wrong */
	int ret_val = Utility::setDate(date, struct_datetime, date_format, date_separator);

	if (ret_val)
		return ret_val;

	ret_val = Utility::setTime(time, struct_datetime, time_format, time_separator);
	if (ret_val)
		return ret_val;

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
	int year = -1, month = -1, day = -1;
	
	while (getline(ss_format, date_attr, '/')) {

		/* if no more date attributes break */
		if(!getline(ss_date, date_attr_value, separator))
			break;

		if (date_attr == "y") {

			/* Check if the year is valid*/
			if (!isNumber(date_attr_value))
				return INVALID_VALUE;

			year = std::atoi(date_attr_value.c_str());

			/* Check if year is non zero */
			if (!year)
				return OUT_OF_RANGE;

		} else if (date_attr == "m") {

			/* Check if the month is valid */
			if (!isNumber(date_attr_value)) 
				return INVALID_VALUE;

			month = std::stoi(date_attr_value.c_str());

			/* Check if month is in range */
			if (month > 12 || month < 1)
				return OUT_OF_RANGE;

		} else {

			/* Check if the day is valid */
			if (!isNumber(date_attr_value)) 
				return INVALID_VALUE;

			day = std::atoi(date_attr_value.c_str());

			/* Check if the day is non-zero */
			if (!day)
				return OUT_OF_RANGE;
		}
	}
	
	if (month == -1 || year == -1 || day == -1) {
		return INVALID_VALUE;
	} else {

		/* Check for valid dates */
		/* Check if is Febrary (Special case) */
		if (month == 2) {
			/* Check if is Leap year */
			if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0)) {
				if (day > 29)
					return OUT_OF_RANGE;
			} else {
				if (day > 28)
					return OUT_OF_RANGE;
			}
		} else if (day > month_days[month]) {
			return OUT_OF_RANGE;
		} 
	}

	/* if no problems found assign values */
	datetime.tm_year = year;
	datetime.tm_mon = month;
	datetime.tm_mday = day;

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
	int hour = -1, minute = -1, second = -1;

	while (getline(ss_format, time_attr, '/')) {
		/* If no more time attributes break */
		if (!getline(ss_time, time_attr_value, separator))
			break;

		if (time_attr == "h") {

			/* Check if attribure is a number */
			if (!isNumber(time_attr_value))
				return INVALID_VALUE;

			hour = std::atoi(time_attr_value.c_str());
		} else if (time_attr == "m") {

			/* Check if attribure is a number */
			if (!isNumber(time_attr_value))
				return INVALID_VALUE;

			minute = std::atoi(time_attr_value.c_str());
		} else {

			/* Check if attribure is a number */
			if (!isNumber(time_attr_value))
				return INVALID_VALUE;

			second = std::atoi(time_attr_value.c_str());
		}
	}

	/* Missing attributes or out of range values*/
	if (hour == -1 || minute == -1 || second == -1) {
		return INVALID_VALUE;
	} else if (hour > 23 || minute > 59 || second > 59) {
		return OUT_OF_RANGE;
	}

	datetime.tm_hour = hour;
	datetime.tm_min = minute;
	datetime.tm_sec = second;

	return SUCCESS;
}

void Utility::trim(std::string &str_value) {

	/* Delete leading spaces */
	while (!str_value.empty() && std::isspace(str_value.back()))
		str_value.pop_back();

	/* Delete trailing spaces */
	while (!str_value.empty() && std::isspace(str_value.front()))
		str_value.erase(str_value.begin());
}

