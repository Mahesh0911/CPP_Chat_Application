// Login.h
#pragma once
#include "DatabaseConnection.h"
#include "User.h"

class Login {
private:
    DatabaseConnection& dbConnection;
    User loggedInUser;

public:
    Login(DatabaseConnection& dbConnection);
    bool executeLogin();
    void signedUser(User user);
    const User& getLoggedInUser() const;
        
};
