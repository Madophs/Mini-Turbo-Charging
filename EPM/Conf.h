#ifndef EMP_CONF_H
#define EMP_CONF_H

#include <iostream>

/* 
 * EPM_Conf : Singleton class
 * 
 * Contains the properties from EPM config file
 *
 */

class EPM_Conf {
	public:
		
		EPM_Conf(const EPM_Conf &) = delete;	

		static EPM_Conf &instance();
		
		static void readConfigFile();

		static std::string getDBName();
		static void setDBName();

		static std::string getDBUsername();
		static void setDBUsername(std::string username);				

		static std::string getDBPassword();
		static void setDBPassword(std::string password);
		
		static std::string getDBService();
		static void setDBService(std::string service);

		static std::string getDBHost();
		static void setDBHost(std::string host_addr);

		static std::string getInputEventFileLocation();
		static void setInputEventFileLocation(std::string location);

		static std::string getProcessedEventFileLocation();
		static void setProcessedEventFileLocation(std::string location);

		static std::string getConfigFileLocation();
		static void setConfigFileLocation(std::string location);

		static unsigned int getDBPort();
		static void setDBPort(unsigned int port);

		static unsigned int getSleepTime();
		static void setSleepTime(unsigned int sleep_time);

	private:
		EPM_Conf();
		std::string config_file_path_;
		std::string db_name_;
		std::string db_user_;
		std::string db_password_;
		std::string db_service_;
		std::string db_host_;
		std::string input_event_file_location_;
		std::string processed_event_file_location_;
		unsigned int db_port_;
		unsigned int sleep_time_;
		void assignProperties(std::string &property_name, std::string &property_value);
};

#endif
