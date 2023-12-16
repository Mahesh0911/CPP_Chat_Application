// Signup.cpp
#include "Signup.h"
#include <iostream>

Signup::Signup(DatabaseConnection& dbConnection) : dbConnection(dbConnection){}

bool Signup::executeSignup() {
    std::string username, password;

    std::cout << "Enter your desired username: ";
    std::cin >> username;

    std::cout << "Enter your password: ";
    std::cin >> password;

    // Check if the username already exists
    std::string checkUserQuery = "SELECT * FROM users WHERE username='" + username + "'";
    MYSQL_RES* result = nullptr;

    if (dbConnection.executeQuery(checkUserQuery) ) {
        result = mysql_store_result(dbConnection.getConnection());

        if (result != nullptr && mysql_num_rows(result) > 0) {
            system("cls");
            std::cerr << "Username already exists. Please choose a different username." << std::endl;
            mysql_free_result(result);
            return false;
        }

        mysql_free_result(result);
    } else {
        std::cerr << "Error checking username: " << mysql_error(dbConnection.getConnection()) << std::endl;
       return false;
    }

    // Insert the new user
    std::string insertUserQuery = "INSERT INTO users (username, password) VALUES ('" + username + "', '" + password + "')";
    if (dbConnection.executeQuery(insertUserQuery)) {
        std::cout << "Signup successful! Welcome, " << username << "!" << std::endl;

        // Fetch the user ID for the newly created user
        std::string getUserIdQuery = "SELECT user_id FROM users WHERE username='" + username + "'";
        if (dbConnection.executeQuery(getUserIdQuery)) {
            result = mysql_store_result(dbConnection.getConnection());

            if (result != nullptr && mysql_num_rows(result) > 0) {
                MYSQL_ROW row = mysql_fetch_row(result);
                int userId = std::stoi(row[0]);
                mysql_free_result(result);
                
            } else {
                std::cerr << "Error fetching user information after signup." << std::endl;
            }
        } else {
            std::cerr << "Error fetching user information after signup: " << mysql_error(dbConnection.getConnection()) << std::endl;
        }

        return true;
    } else {
        std::cerr << "Error during signup: " << mysql_error(dbConnection.getConnection()) << std::endl;
        return false;
    }
}
