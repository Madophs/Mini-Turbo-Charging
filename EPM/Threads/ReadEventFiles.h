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
		int open(void*);
		int svc(void);
		int close(u_long);
	private:
		std::queue<std::string> files_queue;
		CreateEventThread *event_creator_;
		ACE_Message_Block * mb_event_data_;
		void readFiles();
		void addFilesToQueue();
};

#endif
