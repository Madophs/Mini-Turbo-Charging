#pragma once

#include "gtest/gtest.h"
#include "../EPM/Conf.h"
#include "../EPM/EventFactory.h"
#include "../DB/DB.h"
#include "../EPM/Threads/PrepareEventObject.h"
#include "../EPM/EventRate.h"


DB db;
EventFactory event_factory;
RatedEvent rated_event;
PrepareEvent *prepare_event;
Event *event;

void init() {
    EPM_Conf::setConfigFileLocation("/home/madophs/Documents/git/Mini-Turbo-Charging/epm.properties");
    EPM_Conf::readConfigFile();
    db.setDBName(EPM_Conf::getDBName());
    db.setUsername(EPM_Conf::getDBUsername());
    db.setPassword(EPM_Conf::getDBPassword());
    db.setPort(EPM_Conf::getDBPort());
    db.connect();
    prepare_event = nullptr;
    event = nullptr;
    std::cout << std::fixed << std::setprecision(2);
}

void freeMemory() {
    if (event)
        delete event;
    event = nullptr;
}

void i_test(std::string event_data, std::string expected_result) {
    event = event_factory.createEventFromUnparsedData(event_data);
    prepare_event->processEvent(event, rated_event);
    EXPECT_EQ(prepare_event->rate_event_test(rated_event), expected_result);
    freeMemory();
}

TEST(rating_test, test1) {
    init();
    if (!db.isConnected()) {
        std::cerr << "Error: cannot connect to the database " << db.getDBName() << "." << std::endl;
        return;
    }
    /* Load information from database (EVENT_RATE table) */
    EventRateScheme::updateScheme();

    prepare_event = new PrepareEvent();

    /* Test start here */
    i_test("GSM,2532,10/09/2015 10:12:00,200,2025550147", "Hello world");
    i_test("GPRS	,2025550163	,18/09/2015 10:12:00,	1024, facebook.com", "Hello world");
    i_test("SMS,2025550163,18/09/2015 10:12:00,100,2025550147", "Hello world");
    i_test("GSM,2025550166,18/10/2015 10:12:01,175,2027550163", "Hello world");
    i_test("GPRS,2025550168,9/9/2015 10:12:00,5000,stackoverflow.com", "Hello world");
    i_test("GPRS,2025550168,9/9/2015 10:12:00,5000", "Hello world");
    i_test("GSM,2025550166,18/10/2015 10:12:01,175k,2027550163", "Hello world");
    delete prepare_event;
}