// Signup.h
#pragma once
#include "DatabaseConnection.h"
#include "User.h"

class Signup {
private:
    DatabaseConnection& dbConnection;

public:
    Signup(DatabaseConnection& dbConnection);
    bool executeSignup();
   
};
