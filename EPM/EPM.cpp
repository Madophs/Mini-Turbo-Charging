#include "EPM.h"
#include "EventRate.h"

EPM::EPM() {
    db_thread_ = new DBManagement();
	pe_thread_ = new PrepareEvent(db_thread_);
	ce_thread_ = new CreateEventThread(pe_thread_);
	re_thread_ = new ReadEventsThread(ce_thread_);
    this->threads_running_ = false;
}

EPM::~EPM() {
    if (db_thread_)
        delete db_thread_;
    if (pe_thread_)
        delete pe_thread_;
    if (ce_thread_)
        delete ce_thread_;
    if (re_thread_)
        delete re_thread_;
}

void EPM::start() {
    if (threads_running_) return;
    EPM_Conf::setConfigFileLocation("/home/madophs/Documents/git/Mini-Turbo-Charging/epm.properties");
	EPM_Conf::readConfigFile();
    if (!testDBConnection()) {
        std::cerr << "Cannot connect to DB" << std::endl;
        exit(EXIT_FAILURE);
    }
    EventRateScheme::updateScheme();
    db_thread_->open(0);
	pe_thread_->open(0);
	ce_thread_->open(0);
	re_thread_->open(0);
    threads_running_ = true;
}

bool EPM::testDBConnection() {
    DB db;
	db.setDBName(EPM_Conf::getDBName());
	db.setUsername(EPM_Conf::getDBUsername());
	db.setPassword(EPM_Conf::getDBPassword());
	db.setPort(EPM_Conf::getDBPort());
	db.connect();
    return db.isConnected();
}