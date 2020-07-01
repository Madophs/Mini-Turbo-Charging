#include "DB_Management.h"

DBManagement::DBManagement() {
    this->db_ = std::make_unique<DB>();
    this->connectToDB();
}

DBManagement::~DBManagement() {
    if (mb_transaction_file_)
        delete mb_transaction_file_;
}

int DBManagement::open(void *) {
    activate(THR_NEW_LWP, 1);
    return 0;
}

int DBManagement::svc(void) {
    ACE_DEBUG((LM_INFO, "(%t) Starting DB thread\n"));
    while (true) {
        handleInsertionRequest();
        ACE_OS::sleep(1);
    }
    return 0;
}

int DBManagement::close(ulong) {
    ACE_DEBUG((LM_INFO, "(%t) Closing DB thread\n"));
    return 0;
}

void DBManagement::handleInsertionRequest() {
    /* Receive transaction file */
    getq(mb_transaction_file_);
    /* Use Copy command to insert data (is faster than Insert) */
    std::string copy_command = "";
    /* Check if transactional file contains rejected or rated events records*/
    if (ACE_OS::strcmp(mb_transaction_file_->rd_ptr(), "rejected_events.csv") == 0) {
        /* Construct Copy command for reading and inserting rejected events*/
        copy_command = "COPY rejected_event (event_type, target_source, event_start_time, event_unit_consumed, rejected_reason) FROM '"
            + std::string(getenv("PWD")) + "/events/.prepare_events/ready/rejected_events.csv' USING DELIMITERS '|'";
    } else {
        /* Construct Copy command for reading and inserting rated events*/
        copy_command = "COPY rated_event (event_type, target_source, event_start_time, event_unit_consumed, total_charge) FROM '"
            + std::string(getenv("PWD")) + "/events/.prepare_events/ready/rated_events.csv' USING DELIMITERS '|'";
    }
    db_->execSQLStmt(copy_command);
}

void DBManagement::connectToDB() {
    db_->setDBName(EPM_Conf::getDBName());
    db_->setUsername(EPM_Conf::getDBUsername());
    db_->setPassword(EPM_Conf::getDBPassword());
    db_->setPort(EPM_Conf::getDBPort());
    db_->connect();
}
