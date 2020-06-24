#ifndef READ_EVENTS_THREAD_H
#define READ_EVENTS_THREAD_H

#include <iostream>
#include <ace/OS.h>
#include <ace/Task.h>
#include <ace/Message_Block.h>
#include <queue>
#include "../../EPM/Conf.h"
#include "CreateEventObject.h"

class ReadEventsThread : public ACE_Task<ACE_MT_SYNCH> {
	public:

		/* 
		 * ReadEventsThread : Object pointer to which
		 * the information read from event file will be send
		 *
		 */
		ReadEventsThread(CreateEventThread *create_event_thread);
		~ReadEventsThread();
		int open(void*);
		int svc(void);
		int close(u_long);
	private:
		std::queue<std::string> files_queue;
		CreateEventThread *event_creator_;
		void readFiles();
		void addFilesToQueue();
		ACE_thread_t *threads_ids_;
};

#endif
