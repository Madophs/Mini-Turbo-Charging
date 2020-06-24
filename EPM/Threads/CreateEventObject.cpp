#include "CreateEventObject.h"
#define NUMBER_OF_THREADS 2

CreateEventThread::CreateEventThread(PrepareEvent *prepare_event) {
	this->prepare_event_ = prepare_event;
}

int CreateEventThread::open(void *) {
	ACE_DEBUG((LM_INFO, "(%t) Starting Create Event Thread\n"));
	activate(THR_NEW_LWP, NUMBER_OF_THREADS);
	return 0;
}

int CreateEventThread::svc(void) {
	ACE_DEBUG((LM_DEBUG, "(%t) Create Event Thread\n"));

	/* handle all incoming data (infinite loop) */
	handleEventData();

	return 0;
}

int CreateEventThread::close(u_long) {
	ACE_DEBUG((LM_INFO, "(%t) Closing create event thread..."));
	return 0;
}

void CreateEventThread::handleEventData() {
	unsigned int created_events_count = 0;
	ACE_Message_Block *mb_event_data = 0;
	EventFactory event_factory;

	while (true) {
		mb_event_data = nullptr;

		/* Recieve event data */
		getq(mb_event_data);

		/* Create Event Object */
		Event *event = event_factory.createEventFromUnparsedData(mb_event_data->rd_ptr());

		/* Add event to Prepare event queue */
		this->lock_.acquire();
		prepare_event_->event_queue.push(event);
		this->lock_.release();

		if (++created_events_count % 1000 == 0)
			ACE_OS::sleep(1);
	}
	
}
