#include <iostream>
#include "EPM/EPM.h"

using namespace std;

void createTablesSchemes(bool flag = true);

int main() {
	//createTablesSchemes(false);
	EPM::start();
	EPM::wait();
	return 0;
}

void createTablesSchemes(bool flag) {
	DB db;
	EPM_Conf::setConfigFileLocation("/home/madophs/Documents/git/Mini-Turbo-Charging/epm.properties");
	EPM_Conf::readConfigFile();
	db.setPort(EPM_Conf::getDBPort());
	db.setDBName(EPM_Conf::getDBName());
	db.setUsername(EPM_Conf::getDBUsername());
	db.setPassword(EPM_Conf::getDBPassword());
	db.connect();
	string sql = "";
	string sql_drop = "DROP TABLE IF EXISTS event_rate;";
	sql = sql + "CREATE TABLE event_rate(" +
		"id SERIAL  PRIMARY KEY NOT NULL," +
		"event_type VARCHAR(8)," +
		"effective_date DATE," +
		"uom VARCHAR(12)," +
		"unit_amount INTEGER," +
		"unit_rate INTEGER);";
	if (flag) {
		db.execSQLStmt(sql_drop);
		db.execSQLStmt(sql);
	}
	sql_drop = "DROP TABLE IF EXISTS rated_event";
	sql = "";
	sql = sql + "CREATE TABLE rated_event(" +
		"id SERIAL PRIMARY KEY NOT NULL," +
		"event_type VARCHAR(256)," +
		"target_source INTEGER," +
		"event_start_time TIMESTAMP," +
		"event_unit_consumed REAL," +
		"total_charge REAL);";
	db.execSQLStmt(sql_drop);
	db.execSQLStmt(sql);
	sql_drop = "DROP TABLE IF EXISTS rejected_event";
	sql = "";
	sql = sql + "CREATE TABLE rejected_event(" +
		"id SERIAL PRIMARY KEY NOT NULL," +
		"event_type VARCHAR(256)," +
		"target_source INTEGER," +
		"event_start_time TIMESTAMP," +
		"event_unit_consumed REAL," +
		"rejected_reason VARCHAR(256));";
	db.execSQLStmt(sql_drop);
	db.execSQLStmt(sql);
	db.disconnect();
}
