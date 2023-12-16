// Login.cpp
#include "Login.h"
#include <iostream>

Login::Login(DatabaseConnection& dbConnection) : dbConnection(dbConnection){}

bool Login::executeLogin() {
    std::string username, password;

    std::cout << "Enter your username: ";
    std::cin >> username;

    std::cout << "Enter your password: ";
    std::cin >> password;

    std::string loginQuery = "SELECT user_id, username, password FROM users WHERE username='" + username + "' AND password='" + password + "'";
    MYSQL_RES* result = nullptr;

    if (dbConnection.executeQuery(loginQuery)) {
        result = mysql_store_result(dbConnection.getConnection());

        if (result != nullptr && mysql_num_rows(result) > 0) {
            MYSQL_ROW row = mysql_fetch_row(result);
            int userId = std::stoi(row[0]);
            // You may also fetch other user details from the database if needed

            //std::cout << "Login successful!" << std::endl;
            loggedInUser =User(userId, username, password);
            mysql_free_result(result);
            return true;
        }
        else {
            system("cls");
            std::cerr << "Invalid username or password. Please try again." << std::endl;
            mysql_free_result(result);
            return false;
        }
    }
    else {
        std::cerr << "Error during login: " << mysql_error(dbConnection.getConnection()) << std::endl;
        return false;
    }
}

void Login::signedUser(User user) {
    loggedInUser = user;
}

const User& Login::getLoggedInUser() const {
    return loggedInUser;
}
