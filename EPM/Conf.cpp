#include "Conf.h"
#include <fstream>
#include <sstream>
#include "Utils.h"

EPM_Conf::EPM_Conf() {
	this->config_file_path_ = "./epm.properties";
	this->db_name_ = "postgres";
	this->db_user_ = "postgres";
	this->db_password_ = "secret";
	this->db_service_ = "service";
	this->db_host_ = "127.0.0.1";
	this->db_port_ = 5432;
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

	/* Open configuration file (epm.properties) */
	config_file.open(instance().config_file_path_.c_str());
	if (!config_file.fail()) {
		std::string line, property_name, property_value;
		std::stringstream ss;

		/* Read line by line from EPM configuration file */
		while (getline(config_file, line)) {
			ss.str(line);

			/* Separate property name and value */	
			if (!getline(ss, property_name, '=')) {
				std::cerr << "Error: " << line << " is not a valid property" << std::endl;
				continue;
			}
			if (!getline(ss, property_value, '=')) {
				std::cerr << "Error: " << line << " is not a valid property" << std::endl;
				continue;
			}

			instance().assignProperties(property_name, property_value);	
			ss.clear();
		}
		config_file.close();
	} else {
		std::cerr << "EPM Config file not found" << std::endl;	
	}
}

void EPM_Conf::assignProperties(std::string &property_name, std::string &property_value) {

	if (property_name ==	"epm.input.events.location") {
		input_event_file_location_ = property_value;
	} else if (property_name == "epm.sleep.time") {
		if (Utils::isNumber(property_value)) {
			sleep_time_ = std::atoi(property_value.c_str());
		} else {
			std::cerr << "Value not valid" << std::endl;
		}
	} else if (property_name == "epm.db.name") {
		db_name_ = property_value;
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

/* Getters and Setters */

std::string EPM_Conf::getDBUsername() {
	return instance().db_user_;
}

std::string EPM_Conf::getDBName() {
	return instance().db_name_;
}

void EPM_Conf::setDBUsername(std::string username) {
	instance().db_user_ = username;
}

std::string EPM_Conf::getDBPassword() {
	return instance().db_password_;
}

void EPM_Conf::setDBPassword(std::string pass) {
	instance().db_password_ = pass;
}

std::string EPM_Conf::getDBService() {
	return instance().db_service_;
}

void EPM_Conf::setDBService(std::string service) {
	instance().db_service_ = service;
}

std::string EPM_Conf::getDBHost() {
	return instance().db_host_;
}

void EPM_Conf::setDBHost(std::string host) {
	instance().db_host_ = host;
}

std::string EPM_Conf::getInputEventFileLocation() {
	return instance().input_event_file_location_;
}

void EPM_Conf::setInputEventFileLocation(std::string location) {
	instance().input_event_file_location_ = location;
}

std::string EPM_Conf::getProcessedEventFileLocation() {
	return instance().processed_event_file_location_;
}

void EPM_Conf::setProcessedEventFileLocation(std::string location) {
	instance().processed_event_file_location_ = location;
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

void EPM_Conf::setDBPort(unsigned int port) {
	instance().db_port_ = port;
}

unsigned int EPM_Conf::getSleepTime() {
	return instance().sleep_time_;
}

void EPM_Conf::setSleepTime(unsigned int port) {
	instance().sleep_time_ = port;
}

