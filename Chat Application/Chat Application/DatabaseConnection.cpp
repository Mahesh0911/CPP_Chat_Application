// DatabaseConnection.cpp
#include "DatabaseConnection.h"

DatabaseConnection::DatabaseConnection() : connection(nullptr) {}

DatabaseConnection::~DatabaseConnection() {
    if (connection != nullptr) {
        mysql_close(connection);
    }
}

bool DatabaseConnection::connect() {
    connection = mysql_init(nullptr);

    if (connection == nullptr) {
        std::cerr << "mysql_init() failed" << std::endl;
        return false;
    }
    connection = mysql_real_connect(connection, "localhost", "root", "Mahesh@09", "ChatApp", 3306, NULL, 0);
    if ( connection== nullptr) {
        std::cerr << "mysql_real_connect() failed" << std::endl;
        mysql_close(connection);
        return false;
    }

    return true;
}

bool DatabaseConnection::executeQuery(const std::string& query) {
    if (mysql_query(connection, query.c_str()) != 0) {
        std::cerr << "Query execution failed: " << mysql_error(connection) << std::endl;
        return false;
    }
    /*std::cout << "Hello -------";
    std::cout<<connection->passwd;*/

    return true;
}

MYSQL* DatabaseConnection::getConnection() {
    return connection;
}
