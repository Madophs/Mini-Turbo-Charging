#ifndef EMP_CONF_H
#define EMP_CONF_H

#include <iostream>

/* Singleton class
 * 
 * Contains the properties from EPM config file
 *
 */

class EPM_Conf {
	public:
		EPM_Conf(const EPM_Conf &) = delete;	
		static EPM_Conf &instance();
		
		static void readConfigFile();

		static std::string getDBUsername();
		static void setDBUsername(std::string new_username);				

		static std::string getDBPassword();
		static void setDBPassword(std::string new_pass);
		
		static std::string getDBService();
		static void setDBService(std::string new_service);

		static std::string getInputEventFileLocation();
		static void setInputEventFileLocation(std::string new_location);

		static std::string getProcessedEventFileLocation();
		static void setProcessedEventFileLocation(std::string new_location);

		static std::string getConfigFileLocation();
		static void setConfigFileLocation(std::string location);

		static unsigned int getDBPort();
		static void setDBPort(unsigned int new_port);

		static unsigned int getSleepTime();
		static void setSleepTime(unsigned int new_port);

	private:
		EPM_Conf();
		std::string config_file_path_;
		std::string db_user_;
		std::string db_password_;
		std::string db_service_;
		std::string input_event_file_location_;
		std::string processed_event_file_location_;
		unsigned int db_port_;
		unsigned int sleep_time_;
		
		void assignProperties(std::string &property_name,
							  std::string &property_value);
};

#endif
