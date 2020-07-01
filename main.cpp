#include <atomic>
#include <iostream>
#include "EPM/EPM.h"
#include "EPM/Utility.h"

using namespace std;

int main(int argc, char *argv[]) {
	try {
		Utility::parseArgs(argc, argv);
		EPM_Conf::readConfigFile();
		EPM::start();
		EPM::wait();
	} catch (std::exception &e) {
		cout << e.what() << endl;
	}
	return 0;
}

