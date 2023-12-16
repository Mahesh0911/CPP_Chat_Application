// Message.h
#pragma once
#include <string>

class Message {
private:
    int userId;
    std::string senderName;
    std::string messageText;
    std::string timestamp;

public:
    Message(int userId, const std::string& senderName, const std::string& messageText, const std::string& timestamp);
    void display() const;
};

