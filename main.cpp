#include <iostream>
#include "EPM/Conf.h"

using namespace std;

int main() {
	std::cout << "Esperemos que esto funciones" << std::endl;
	EPM_Conf::readConfigFile();
	cout << "Username " << EPM_Conf::getDBUsername() << endl;
	cout << "Password " << EPM_Conf::getDBPassword() << endl;
	cout << "Service " << EPM_Conf::getDBService() << endl;
	cout << "Sleep time " << EPM_Conf::getSleepTime() << endl;
	cout << "Input " << EPM_Conf::getInputEventFileLocation() << endl;
	cout << "Processed location " << EPM_Conf::getProcessedEventFileLocation() << endl;
	return 0;
}
