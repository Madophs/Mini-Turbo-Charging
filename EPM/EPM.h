#ifndef EPM_MODULE_H
#define EPM_MODULE_H

#include "Conf.h"
#include "Threads/ReadEventFiles.h"
#include "Threads/CreateEventObject.h"
#include "Threads/PrepareEventObject.h"
#include "Threads/DB_Management.h"

class EPM {
    public:
        EPM();
        ~EPM();
        void start();
    private:
        bool threads_running_;
        DBManagement *db_thread_;
        PrepareEvent *pe_thread_;
        CreateEventThread *ce_thread_;
        ReadEventsThread *re_thread_;
        bool testDBConnection();
};

#endif