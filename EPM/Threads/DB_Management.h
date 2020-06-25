#ifndef DB_MANAGMENT_H
#define DB_MANAGMENT_H

#include <iostream>
#include <ace/OS.h>
#include <ace/Task.h>
#include <ace/Message_Block.h>
#include <memory>
#include "../../DB/DB.h"

/*
 * Receive and execute SQL statement continuously
 */
class DBManagement : public ACE_Task<ACE_MT_SYNCH> {
	public:
		DBManagement();
		~DBManagement();

		/* Start thread */
		int open(void *);
		/* Thread entry point */
		int svc(void);
		int close(u_long);
	private:
        /* SQL statement to be executed */
		ACE_Message_Block *mb_sql_stmt_;
        /* Database pointer */
        std::unique_ptr<DB> db_;
        
		/* 
		 * Receive sql statement ready for insertion.
         * 
		 */
        void handleInsertionRequest();

		/* Establish connection with the DB */
		void connectToDB();
};

#endif
