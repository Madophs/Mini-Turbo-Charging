#include "EventRate.h"
#include "Utility.h"
#include <algorithm>

EventRateScheme::EventRateScheme() {}

EventRateScheme &EventRateScheme::instance() {
	static EventRateScheme i_instance;
	return i_instance;
}

bool EventRateScheme::findEffectiveDate(EventRate &event_rate_search) {
	EventRate ER_search = event_rate_search;
	bool eff_date_found = false;

	/* Linear scan, find the effective date */
	for (EventRate &event_rate_iter : instance().event_rate_rec_) {

		/* Check if event types are the same */
		if (ER_search.getEventType() == event_rate_iter.getEventType()) {
			/*
			 * Check if effective date is less , this means that can be applied to our current event date
			 * but we need to find the closest one
			 */
			if (event_rate_iter <= ER_search) {
				/* if effective date found update the event_rate_search */
				event_rate_search = event_rate_iter;
				eff_date_found = true;
			}
		}
	}
	return eff_date_found;
}

void EventRateScheme::updateScheme() {
	instance().iUpdateScheme();
}

std::vector<EventRate> EventRateScheme::getRecords() {
	return instance().event_rate_rec_;
}

void EventRateScheme::iUpdateScheme() {

	/* Clear previous records */
	event_rate_rec_.clear();
	
	/* Create a database connection */
	DB db;
	db.setDBName(EPM_Conf::getDBName());
	db.setUsername(EPM_Conf::getDBUsername());
	db.setPassword(EPM_Conf::getDBPassword());
	db.setPort(EPM_Conf::getDBPort());
	db.connect();

	/* Select from event_rate table */
	pqxx::result results = db.select("event_rate");

	/* Event rate fields */
	std::string event_type, effective_date_str, uom;
	int unit_amount, unit_rate;

	/* Iterate through records */
    for (pqxx::result::const_iterator record = results.begin(); record != results.end(); ++record) {

		/* Get and parse event rate values */
		event_type = record[1].as<std::string>();
		effective_date_str = record[2].as<std::string>();
		tm effective_date;
		Utility::setDate(effective_date_str, effective_date,"y/m/d",'-');
		uom = record[3].as<std::string>();
		unit_amount = record[4].as<int>();
		unit_rate = record[5].as<int>();

		/* Create Event Rate object */
		EventRate event_rate;
		event_rate.setEventType(event_type);
		event_rate.setEffectiveDate(effective_date);
		event_rate.setUOM(uom);
		event_rate.setUnitAmount(unit_amount);
		event_rate.setUnitRate(unit_rate);

		/* Add event_rate object to event_rate_records vector */
		event_rate_rec_.push_back(event_rate);
    }
	
	/* Sort records based on effective and event_type criteria */
	sort(event_rate_rec_.begin(), event_rate_rec_.end());

	/* Close connection */
	db.disconnect();
}


