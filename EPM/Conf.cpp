#include "Conf.h"
#include <fstream>
#include <sstream>

EPM_Conf::EPM_Conf() {
	this->config_file_path_ = "./epm.properties";
	this->db_user_ = "root";
	this->db_password_ = "secret";
	this->db_service_ = "service";
	this->db_port_ = 2532;
	this->input_event_file_location_ = "./files/input/";
	this->processed_event_file_location_ = "./files/processed";
	this->sleep_time_ = 3;
}

EPM_Conf &EPM_Conf::instance() {
	static EPM_Conf ins;
	return ins;
}

void EPM_Conf::readConfigFile() {
	std::ifstream config_file;
	config_file.open(instance().config_file_path_.c_str());
	if (!config_file.fail()) {
		std::string line, property_name, property_value;
		std::stringstream ss;

		/* Read line by line from epm configuration file */
		while (getline(config_file, line)) {
			ss.str(line);
			if (!getline(ss, property_name, '=')) {
				std::cout << "Algo paso aqui" << std::endl;
				continue;
			}
			if (!getline(ss, property_value, '=')) {
				std::cout << "Algo paso aqui" << std::endl;
				continue;
			}
			instance().assignProperties(property_name, property_value);	
			std::cout << property_name << " " << property_value << std::endl;
			ss.clear();
		}
		config_file.close();
	} else {
		config_file.close();
	}
}

void EPM_Conf::assignProperties(std::string &property_name, std::string &property_value) {
	if (property_name ==	"epm.input.events.location") {
		input_event_file_location_ = property_value;
	} else if (property_name == "epm.sleep.time") {
		bool is_number = true;
		for (auto &character : property_value) {
			if(!std::isdigit(character)) {
				is_number = false;
				break;
			}
		}	

		if (is_number) {
			sleep_time_ = std::atoi(property_value.c_str());
		} else {
			std::cerr << "Value not valid" << std::endl;
		}
			
	} else if (property_name == "epm.db.user") {
		db_user_ = property_value;
	} else if (property_name == "epm.db.password") {
		db_password_ = property_value;
	} else if (property_name == "epm.db.service") {
		db_service_ = property_value;
	} else if (property_name == "epm.processed.events.location") {
		processed_event_file_location_ = property_value;
	} else {
		std::cerr << "Property: " << property_name << " is not valid" << std::endl;
	}
}

/* EPM properties getters and setters */

std::string EPM_Conf::getDBUsername() {
	return instance().db_user_;
}

void EPM_Conf::setDBUsername(std::string new_username) {
	instance().db_user_ = new_username;
}

std::string EPM_Conf::getDBPassword() {
	return instance().db_password_;
}

void EPM_Conf::setDBPassword(std::string new_pass) {
	instance().db_password_ = new_pass;
}

std::string EPM_Conf::getDBService() {
	return instance().db_service_;
}

void EPM_Conf::setDBService(std::string new_service) {
	instance().db_service_ = new_service;
}

std::string EPM_Conf::getInputEventFileLocation() {
	return instance().input_event_file_location_;
}

void EPM_Conf::setInputEventFileLocation(std::string new_location) {
	instance().input_event_file_location_ = new_location;
}

std::string EPM_Conf::getProcessedEventFileLocation() {
	return instance().processed_event_file_location_;
}

void EPM_Conf::setProcessedEventFileLocation(std::string new_location) {
	instance().processed_event_file_location_ = new_location;
}

std::string EPM_Conf::getConfigFileLocation() {
	return instance().config_file_path_;
}

void EPM_Conf::setConfigFileLocation(std::string location) {
	instance().config_file_path_ = location;
}

unsigned int EPM_Conf::getDBPort() {
	return instance().db_port_;
}

void EPM_Conf::setDBPort(unsigned int new_port) {
	instance().db_port_ = new_port;
}

unsigned int EPM_Conf::getSleepTime() {
	return instance().sleep_time_;
}

void EPM_Conf::setSleepTime(unsigned int new_port) {
	instance().sleep_time_ = new_port;
}

