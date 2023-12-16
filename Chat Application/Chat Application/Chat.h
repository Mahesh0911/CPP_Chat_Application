// Chat.h
#pragma once
#include "DatabaseConnection.h"
#include "User.h"
#include "Message.h"

class Chat {
private:
    DatabaseConnection& dbConnection;
    const User& loggedInUser;
    const User& targetUser;

public:
    Chat(DatabaseConnection& dbConnection, const User& loggedInUser, const User& targetUser);
    void startChat();
    void sendMessage(const std::string& text) const;
    void displayMessages() const;
    std::vector<Message> getMessages() const;
};
