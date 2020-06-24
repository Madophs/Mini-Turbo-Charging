#include "ReadEventFiles.h"
#include <fstream>
#include <dirent.h>
#include <cstdio>
#include <vector>
#define NUMBER_OF_THREADS 1

ReadEventsThread::ReadEventsThread(CreateEventThread *create_event_thread) {
	this->event_creator_ = create_event_thread;
	this->threads_ids_ = new ACE_thread_t[NUMBER_OF_THREADS];
}

ReadEventsThread::~ReadEventsThread() {
	if (threads_ids_)
		delete[] threads_ids_;
}

int ReadEventsThread::open(void *) {
	activate(THR_NEW_LWP, NUMBER_OF_THREADS, 0, ACE_DEFAULT_THREAD_PRIORITY, -1, 0, 0, 0, 0, threads_ids_);
	return 0;
}

int ReadEventsThread::svc(void) {
	ACE_DEBUG((LM_DEBUG, "(%t) Read Event Thread\n"));
	while (true) {
		addFilesToQueue();
		/* if no files to read, just take a nap */
		if (files_queue.empty()) {
			ACE_OS::sleep(EPM_Conf::getSleepTime());
		} else {
			readFiles();
		}
	}
	return 0;
}

int ReadEventsThread::close(u_long) {
	ACE_DEBUG((LM_INFO, "(%t) Closing read thread\n"));
	return 0;
}

void ReadEventsThread::readFiles() {

	std::ifstream file_reader;
	std::string line;
	
	/* Message block for sending the event data */
	ACE_Message_Block *mb_event_data;
	
	/* Proccess files in the queue */
	while (!files_queue.empty()) {

		/* filename from queue */
		std::string filename = files_queue.front();
		files_queue.pop();

		/* Open event file */
		std::string event_file_path = EPM_Conf::getInputEventFileLocation() + "/" + filename;
		file_reader.open(event_file_path);

		/* Variable useful to sleep this thread after read N number of lines */
		int read_lines = 0;

		while (getline(file_reader, line)) {

			/* Skip empty lines */
			if (line.empty()) continue;

			/* Skip lines containing only spaces */
			if (line.find_last_of(' ') == line.length()-1) continue;

			/* Buffer with the event data */
			char *buf_event_data = new char[line.length()+1];

			ACE_OS::memset(buf_event_data, 0, sizeof(buf_event_data));

			/* Copying the content from files to the buffer */
			strcpy(buf_event_data, line.c_str());

			mb_event_data = 0;
			mb_event_data = new ACE_Message_Block(buf_event_data, sizeof(buf_event_data));

			/* Enqueue event data to CreateEventThread */
			event_creator_->putq(mb_event_data);

			if (++read_lines % 10000 == 0)
				ACE_OS::sleep(1);
		}
		file_reader.clear();
		file_reader.close();

		/* construct move command */
		std::string move_files_command = "mv " + event_file_path + " " + EPM_Conf::getProcessedEventFileLocation();
		
		/* Execute mv command */
		system(move_files_command.c_str());
	}
}

void ReadEventsThread::addFilesToQueue() {
	DIR *dir;

	/* Directory stream */
	dirent *dir_stream;

	if ((dir = opendir(EPM_Conf::getInputEventFileLocation().c_str())) != NULL) {
		
		/* Read directory files */
		while ((dir_stream = readdir(dir)) != NULL) {
			/* Skip directories */
			if (dir_stream->d_type == DT_DIR) continue;
			/* Skip hidden files */
			if (dir_stream->d_name[0] == '.') continue;
			/* Add the filename to queue */
			files_queue.push(dir_stream->d_name);
		}
	} else {
		ACE_DEBUG((LM_ERROR, "Event directory not found\n"));
		return;
	}
	closedir(dir);
}
