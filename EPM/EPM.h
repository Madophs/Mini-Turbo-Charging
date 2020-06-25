#ifndef EPM_MODULE_H
#define EPM_MODULE_H

#include "Conf.h"
#include "Threads/ReadEventFiles.h"
#include "Threads/CreateEventObject.h"
#include "Threads/PrepareEventObject.h"
#include "Threads/DB_Management.h"

/*
 * Singleton class that implements the Event Process Module funcionability
 */
class EPM {
    public:
		EPM(const EPM &epm) = delete;
        ~EPM();

		static EPM& instance() {
			static EPM s_instance;
			return s_instance;
		}

		/*
		 * Starts read, create, prepare and database threads
		 */
        static void start();
		
		/*
		 * Joins (waits) read, create, prepare and database threads
		 */
		static void wait();

    private:
		/* Keep constructor private to avoid multiple instances */
        EPM();
        bool threads_running_;
		bool threads_joinable_;

		/* events and db threads */
        DBManagement *db_t_;
        PrepareEvent *prepare_events_t_;
        CreateEventThread *create_events_t_;
        ReadEventsThread *read_events_t_;

		/* IMPORTANT - test db connection before start threads */
        bool testDBConnection();

		/* internal methods */

		void i_start();

		void i_wait();
};

#endif
