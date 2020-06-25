#include "EPM.h"
#include "EventRate.h"

EPM::EPM() {
    this->db_t_ = new DBManagement();
	this->prepare_events_t_ = new PrepareEvent(db_t_);
	this->create_events_t_ = new CreateEventThread(prepare_events_t_);
	this->read_events_t_ = new ReadEventsThread(create_events_t_);
    this->threads_running_ = false;
	this->threads_joinable_ = true;
}

EPM::~EPM() {
    if (db_t_)
        delete db_t_;
    if (prepare_events_t_)
        delete prepare_events_t_;
    if (create_events_t_)
        delete create_events_t_;
    if (read_events_t_)
        delete read_events_t_;
}

void EPM::start() {
	instance().i_start();
}

void EPM::wait() {
	instance().i_wait();
}

bool EPM::testDBConnection() {
    DB db;
	db.setDBName(EPM_Conf::getDBName());
	db.setUsername(EPM_Conf::getDBUsername());
	db.setPassword(EPM_Conf::getDBPassword());
	db.setPort(EPM_Conf::getDBPort());
	db.connect();
	bool is_db_connected = db.isConnected();

	/* close db connection */
	db.disconnect();
    return is_db_connected;
}

void EPM::i_start() {
    if (threads_running_) return;
    if (!testDBConnection()) {
        std::cerr << "Cannot connect to DB" << std::endl;
        exit(EXIT_FAILURE);
    }
    EventRateScheme::updateScheme();
    db_t_->open(0);
	prepare_events_t_->open(0);
	create_events_t_->open(0);
	read_events_t_->open(0);
    threads_running_ = true;
}

void EPM::i_wait() {
	/* first check if the threads are running and are joinable */
	if (threads_running_ && threads_joinable_) {
		/* Wait for threads to finish */
		db_t_->wait();
		prepare_events_t_->wait();
		create_events_t_->wait();
		read_events_t_->wait();

		/* Threads are not joinable anymore */
		threads_joinable_ = false;
	}
}
