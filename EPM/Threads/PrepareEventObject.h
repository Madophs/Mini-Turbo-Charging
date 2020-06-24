#ifndef PREPARE_OBJECT_THREAD
#define PREPARE_OBJECT_THREAD

#include <iostream>
#include <ace/OS.h>
#include <ace/Task.h>
#include <ace/Message_Block.h>
#include <queue>
#include "../Event.h"
#include "DB_Management.h"

/* forward declaration */
struct RatedEvent;

class PrepareEvent : public ACE_Task<ACE_MT_SYNCH> {
	public:
		PrepareEvent(DBManagement *db_management);
		PrepareEvent();
		int open(void*);
		int svc(void);
		int close(u_long);
		void process_event(Event *event, RatedEvent &rated_event);
		std::string rate_event_test(RatedEvent &rated_event);
		
		/* Underlying queue to receive event object pointers */
		std::queue<Event *> event_queue;
	private:
		DBManagement *db_management_;
		void process_queue_events();
		void addToDBQueue(std::string &sql_stmt);
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