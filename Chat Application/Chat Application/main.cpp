#include <iostream>
#include "DatabaseConnection.h"
#include "Login.h"
#include "Signup.h"
#include "Chat.h"


//display the main page of the system
Login mainPage(DatabaseConnection& dbConnection) {
    
    Login login(dbConnection);
    
    while (1) {
        //system("cls");
        // Get user choice: signup or login
        std::cout << "1. Signup\n2. Login\n3. Exit" << std::endl;
        std::cout << "Choose an option: " << std::endl;
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            // Signup
            Signup signup(dbConnection);
            if (signup.executeSignup()) {
                //std::cout << "Signup successful!" << std::endl;
                system("cls");
                std::cout << "Please Login to your account."<<std::endl;
            }
            else {
                std::cerr << "Signup failed." << std::endl;

            }
        }
        else if (choice == 2) {

            //Login
            if (login.executeLogin()) {
                std::cout << "Login successful!" << std::endl;
                return login;
            }
            else {
                std::cerr << "Login failed." << std::endl;
            }

        }
        else if (choice == 3) {
            std::cout << "Thank you for visiting. See you, Again..." << std::endl;
            exit(0);
        }
        else {
            system("cls");
            std::cerr << "Invalid Choice !\nTry again..." << std::endl;
            continue;
        }

    }
    
}


//displays the list of users after login
void displayUserList(DatabaseConnection& dbConnection, User& loggedInUser) {
    std::cout << "Welcome "<<loggedInUser.getUsername()<<std::endl;
    std::cout << "Users you have chatted with before:\n";
    std::vector<User> previousChatUsers = loggedInUser.getPreviousChatUsers(dbConnection);
    for (const auto& user : previousChatUsers) {
        std::cout << user.getUsername() << "\n";
    }
    std::cout << "\nEnter the username to start a chat (type 'logout' to logout)";
}

//page to display after login
void loggedInPage(DatabaseConnection& dbConnection, Login login) {
    User loggedInUser = login.getLoggedInUser();
    system("cls");
    while (true) {
        displayUserList(dbConnection, loggedInUser);

        std::string userInput;
        std::cout << "\nEnter your choice: ";
        std::cin >> userInput;
        if (userInput == "logout") {
            std::cout << "Logging out...\n";
            system("cls");
            return;
        }
        else {
            // Check if the user exists
            User targetUser = User::getUserByUsername(dbConnection, userInput);

            if (targetUser.getId() != -1) {
                // Start a new chat
                Chat chat(dbConnection, loggedInUser, targetUser);
                system("cls");
                chat.startChat();
                system("cls");
                continue;
            }
            else {
                system("cls");
                std::cerr << "User with username '" << userInput << "' does not exist." << std::endl;
            }
        }

    }
}

int main() {
    // Establish a connection to the database
    DatabaseConnection dbConnection;
    if (!dbConnection.connect()) {
        std::cerr << "Failed to connect to the database." << std::endl;
        return 1;
    }
    while (1) {

        //get login from mainPage function
        Login login=mainPage(dbConnection) ;
    
        loggedInPage(dbConnection, login);
    }

    return 0;
}