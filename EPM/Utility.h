#ifndef UTILITY_CLASS_H
#define UTILITY_CLASS_H

#include <iostream>

namespace Utility {
	bool isNumber(std::string value);

	/*
	 * Set Datetime attributes to the give datetime struct
	 *
	 * Example: 30/05/2020 10:11:12 d/m/y h/m/s
	 *
	 */

	int setDatetime(std::string datetime,
					tm &struc__datetime,
					std::string format = "y/m/d h/m/s",
					char date_separator = '/',
					char time_separator = ':');

	/*
	 * Sets time attributes to the given struct (reference)
	 *
	 * Time format. Example: h:m:s
	 *
	 */

	int setTime(std::string time, tm &struct_datetime, std::string format = "h/m/s", char separator = ':');

	/*
	 * Sets date attributes to the give struct
	 *
	 * Date format. Example: y/m/d
	 */

	int setDate(std::string date, tm &struct_datetime, std::string format = "y/m/d", char separator = '/');

	/*
	 * Trims trailing and leading whitepaces from a std::string
	 *
	 */
	
	void trim(std::string &str_value);
}

#endif
