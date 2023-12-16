// Message.cpp
#include "Message.h"
#include <iomanip>
#include <iostream>

Message::Message(int userId, const std::string& senderName, const std::string& messageText, const std::string& timestamp)
    : userId(userId), senderName(senderName), messageText(messageText), timestamp(timestamp) {}

void Message::display() const {
    std::cout << std::left << std::setw(10) << senderName << ": " << std::left << std::setw(40)  <<messageText << "\t" << timestamp << std::endl;

}
