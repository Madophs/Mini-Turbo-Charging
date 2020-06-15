#include "ReadEventFiles.h"
#include <fstream>
#include <dirent.h>
#include <cstdio>
#define BUFFER_SIZE 256

ReadEventsThread::ReadEventsThread(CreateEventThread *create_event_thread) {
	this->event_creator_ = create_event_thread;
}

int ReadEventsThread::open(void *) {
	activate(THR_NEW_LWP, 1);
	return 0;
}

int ReadEventsThread::svc(void) {
	addFilesToQueue();
	readFiles();
	return 0;
}

int ReadEventsThread::close(u_long) {
	std::cout << "Closing thread" << std::endl;
	return 0;
}

void ReadEventsThread::readFiles() {
	std::ifstream file_reader;
	std::string line;
	
	/* Proccess files in the queue */
	while (!files_queue.empty()) {
		std::string filename = files_queue.front();
		files_queue.pop();

		/* Open event file */
		std::string file_path = EPM_Conf::getInputEventFileLocation() + "/" + filename;
		file_reader.open(file_path);

		/* Buffer with the event data */
		char *buf_event_data = new char[BUFFER_SIZE];

		while (getline(file_reader, line)) {
			/* Skip empty lines */
			if (line.empty()) continue;
			/* Skip lines containing only spaces */
			if (line.find_last_of(' ') == line.length()-1) continue;

			ACE_OS::memset(buf_event_data, 0, BUFFER_SIZE);

			/* Copying the content from files to the buffer */
			strcpy(buf_event_data, line.c_str());

			//std::cout << "This is the line " << data << std::endl;	
			mb_event_data_ = 0;
			mb_event_data_ = new ACE_Message_Block(buf_event_data, BUFFER_SIZE);

			/* Enqueue event data to CreateEventThread */
			event_creator_->putq(mb_event_data_);
		}
		file_reader.clear();
		file_reader.close();
	}
	ACE_OS::sleep(1);
}

void ReadEventsThread::addFilesToQueue() {

	DIR *dir;
	/* Directory stream */
	dirent *dir_stream;

	if ((dir = opendir(EPM_Conf::getInputEventFileLocation().c_str())) != NULL) {
		while ((dir_stream = readdir(dir)) != NULL) {
			/* Skip directories */
			if (dir_stream->d_type == DT_DIR) continue;
			/* Skip hidden files */
			if (dir_stream->d_name[0] == '.') continue;
			/* Add the filename to queue */
			files_queue.push(dir_stream->d_name);
		}
	} else {
		std::cerr << "Event directory not found" << std::endl;
		return;
	}
	closedir(dir);
}
