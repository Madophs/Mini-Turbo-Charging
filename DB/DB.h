#ifndef DB_CLASS_H
#define DB_CLASS_H
#include <iostream>
#include <pqxx/pqxx>
#include <memory>
#include "../EPM/Conf.h"

/*
 * Implements Database management functionability
 */
class DB {
	public:

		DB();
		~DB();

		void connect();
		void disconnect();
		bool isConnected();

		/* Execute SQL statement */
		void execSQLStmt(std::string sql_stmt);

		/*
		 * @return connection reference
		 */
		pqxx::connection &getConnection();

		/*
		 * @param table - name of table to be consulted
		 *
		 * @return postgreSQL result object containg all the records in the table
		 */
		pqxx::result select(std::string table);
		
		std::string getDBName() const;
		void setDBName(std::string db_name);

		std::string getUsername() const;
		void setUsername(std::string username);

		std::string getPassword() const;
		void setPassword(std::string password);
		
		std::string getService() const;
		void setService(std::string service);

		std::string getHost() const;
		void setHost(std::string host_addr);

		unsigned int getPort() const;
		void setPort(unsigned int port);

	private:
		/* Database connection pointer */
		std::unique_ptr<pqxx::connection> conn_;

		/* Database required field to establish connection */
		std::string db_name_;
		std::string db_user_;
		std::string db_password_;
		std::string db_service_;
		std::string db_host_;
		unsigned int db_port_;
};

#endif
