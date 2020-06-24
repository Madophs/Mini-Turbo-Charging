add_subdirectory(Threads)
add_library(EPM_Conf Conf.cpp Utility.cpp)
add_library(EPM_Events Event.cpp GSM.cpp SMS.cpp GPRS.cpp EventFactory.cpp EventRate.cpp EventRateScheme.cpp EPM.h)
add_library(EPM_Module EPM.cpp)
