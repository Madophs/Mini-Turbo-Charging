#ifndef CREATE_EVENT_THREAD_H
#define CREATE_EVENT_THREAD_H

#include <iostream>
#include <ace/OS.h>
#include <ace/Task.h>
#include <ace/Message_Block.h>
#include "../EventFactory.h"
#include "PrepareEventObject.h"

class CreateEventThread : public ACE_Task<ACE_MT_SYNCH> {
	public:
		CreateEventThread(PrepareEvent *prepare_event);
		int open(void *);
		int svc(void);
		int close(u_long);
	private:
		PrepareEvent *prepare_event_;

		/* 
		 * Receive event data.
		 * Create event object.
		 * Send event object to PrepareObjectThread
		 */
		void handleEventData();
};

#endif
