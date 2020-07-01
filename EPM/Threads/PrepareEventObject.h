#ifndef PREPARE_OBJECT_THREAD
#define PREPARE_OBJECT_THREAD

#include <iostream>
#include <fstream>
#include <ace/OS.h>
#include <ace/Task.h>
#include <ace/Message_Block.h>
#include <queue>
#include "../Event.h"
#include "DB_Management.h"

/* forward declaration */
struct RatedEvent;

/*
 * Rate and prepare the events
 */
class PrepareEvent : public ACE_Task<ACE_MT_SYNCH> {
	public:
		PrepareEvent(DBManagement *db_management);
		PrepareEvent();
		
		/*
		 * Set the DB Management to with rated events will be sent
		 */
		void setDBManagement(DBManagement *db_management);

		/* Spawn threads */
		int open(void*);

		/* Thread entry point */
		int svc(void);
		int close(u_long);

		/* 
		 * Rate the events and set the values in rated_event struct
		 */
		void processEvent(Event *&event, RatedEvent &rated_event);

		/*
		 * This is function is only for testing purposes
		 *
		 * @return a rated event info
		 */
		std::string rate_event_test(RatedEvent &rated_event);
		
		/* Underlying queue to receive event object pointers */
		std::queue<Event *> event_queue;
	private:
		/* Current number of rejected events in transaction file */
		int n_curr_rejected_events_;
		/* Current number of rated events in transaction file */
		int n_curr_rated_events_;
		
		/* Total number of rejected events */
		int n_rejected_events_;
		/* Total number of rated events */
		int n_rated_events_;

		/* Filewriters for rated and rejected events */
		std::ofstream rated_event_writer_;
		std::ofstream rejected_event_writer_;

		DBManagement *db_management_;
		void prepareEvents();
		void processQueueEvents();
		void addToDBQueue(std::string &sql_stmt);
		/* Enqueue and creates a new rejected events transaction file */
		void processRejectedEventFile();
		/* Enqueue and create a new rated events transaction file */
		void processRatedEventFile();
		std::string generateSQLStmt(RatedEvent &rated_event, Event *&event);

};

/* Struct containing rated event info */
struct RatedEvent {
	std::string event_type;
	unsigned int target_source;
	tm start_time;
	float event_unit_consumed;
	float total_charge;
	bool accepted;
	std::string rejected_reason;
};

#endif
