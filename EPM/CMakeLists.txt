add_subdirectory(Threads)
add_library(EPM_Conf Conf.cpp Utils.cpp)
add_library(EPM_Events Event.cpp GSM.cpp SMS.cpp GPRS.cpp EventFactory.cpp EventRate.cpp EventRateScheme.cpp)
