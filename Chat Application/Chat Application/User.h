// User.h
#pragma once
#include <string>
#include <vector>
#include "DatabaseConnection.h"

class User {
private:
    int userId;
    std::string username;
    std::string password;

public:
    User();
    User(int userId, const std::string& username, const std::string& password);
    int getId() const;
    std::string getUsername() const;
    std::string getPassword() const;
    std::vector<User> getPreviousChatUsers(DatabaseConnection& dbConnection) const;
    int getDefaultChatId(DatabaseConnection& dbConnection, const User& targetUser) const;
    int createNewChat(DatabaseConnection& dbConnection, const User& targetUser) const;
    static User getUserByUsername(DatabaseConnection& dbConnection, const std::string& targetUsername);
};
