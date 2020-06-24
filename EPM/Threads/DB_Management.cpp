#include "DB_Management.h"

DBManagement::DBManagement() {
    this->db_ = std::make_unique<DB>();
    this->connectToDB();
}

DBManagement::~DBManagement() {
    if (mb_sql_stmt_)
        delete mb_sql_stmt_;
}

int DBManagement::open(void *) {
    ACE_DEBUG((LM_DEBUG, "(%t) Starting DB thread\n"));
    activate(THR_NEW_LWP, 1);
    return 0;
}

int DBManagement::svc(void) {
    while (true)
        handleInsertionRequest();
    return 0;
}

int DBManagement::close(ulong) {
    ACE_DEBUG((LM_DEBUG, "(%t) Closing DB thread\n"));
    return 0;
}

void DBManagement::handleInsertionRequest() {
    mb_sql_stmt_ = nullptr;
    /* Receive SQL statement ready for insertion */
    getq(mb_sql_stmt_);
    db_->execSQLStmt(mb_sql_stmt_->rd_ptr());
}

void DBManagement::connectToDB() {
    db_->setDBName(EPM_Conf::getDBName());
    db_->setUsername(EPM_Conf::getDBUsername());
    db_->setPassword(EPM_Conf::getDBPassword());
    db_->setPort(EPM_Conf::getDBPort());
    db_->connect();
}