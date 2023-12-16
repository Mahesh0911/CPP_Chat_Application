// DatabaseConnection.h
#pragma once
#include <mysql.h>
#include <iostream>

class DatabaseConnection {
private:
    MYSQL* connection;

public:
    DatabaseConnection();
    ~DatabaseConnection();
    bool connect();
    bool executeQuery(const std::string& query);
    MYSQL* getConnection();
};
