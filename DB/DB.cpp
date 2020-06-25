#include "DB.h"

DB::DB() {
	this->db_user_ = "root";
	this->db_host_ = "127.0.0.1";
	this->db_name_ = "";
	this->db_password_ = "";
	this->db_port_ = 5432;
	this->db_service_ = "";
}

DB::~DB() {
	this->disconnect();
}

void DB::connect() {
	try {
		if (isConnected()) {
			std::cerr << "Connection has been already established" << std::endl;
			return;
		}

		/* Connection statement */
		std::string conn_stmt = "dbname = " + getDBName() +
								" user = " + getUsername() +
								" password = " + getPassword() +
								" hostaddr = " + getHost() +
								" port = " + std::to_string(getPort());

		/* Establish database connection */
		conn_ = std::make_unique<pqxx::connection>(conn_stmt);
	
		if (!conn_->is_open()) {
			std::cout << "Can't open database" << std::endl;
		}
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}

void DB::disconnect() {
	if (conn_ != nullptr && conn_->is_open())
		conn_->disconnect();
}

bool DB::isConnected() {
	return conn_ != nullptr && conn_->is_open();
}

pqxx::connection &DB::getConnection() {
	return *conn_;
}

void DB::execSQLStmt(std::string sql_stmt) {
	if (!isConnected()) {
		std::cerr << "Please established a connection first" << std::endl;
	} else {
		/* Create a transactional object */
		pqxx::work transaction(*conn_);

		/* Execute SQL query */
		transaction.exec(sql_stmt);
		transaction.commit();
	}
}

pqxx::result DB::select(std::string table) {
	if (!isConnected()) {
		throw "Please established a connection first.\n";
	}
	/* Create SQL statement */
	std::string sql_stmt = "SELECT * FROM " + table;
	
	/* Createa non-transactional object */
	pqxx::nontransaction non_transaction(*conn_);

	/* Execute SQL Query */
	pqxx::result results(non_transaction.exec(sql_stmt));
	return results;
}

/* Getters and Setters */

std::string DB::getDBName() const {
	return this->db_name_;
}

void DB::setDBName(std::string database_name) {
	this->db_name_ = database_name;
}

std::string DB::getUsername() const {
	return this->db_user_;
}

void DB::setUsername(std::string username) {
	this->db_user_ = username;
}

std::string DB::getPassword() const {
	return this->db_password_;
}

void DB::setPassword(std::string password) {
	this->db_password_ = password;
}

std::string DB::getService() const {
	return this->db_service_;
}

void DB::setService(std::string service) {
	this->db_service_ = service;
}

std::string DB::getHost() const {
	return this->db_host_;
}

void DB::setHost(std::string host) {
	this->db_host_ = host;
}

unsigned int DB::getPort() const {
	return this->db_port_;
}

void DB::setPort(unsigned int port) {
	this->db_port_ = port;
}

