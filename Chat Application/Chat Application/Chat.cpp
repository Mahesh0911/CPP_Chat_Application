// Chat.cpp
#include "Chat.h"
#include <iostream>

Chat::Chat(DatabaseConnection& dbConnection, const User& loggedInUser, const User& targetUser)
    : dbConnection(dbConnection), loggedInUser(loggedInUser), targetUser(targetUser) {}

void Chat::startChat() {

    while (true) {
        Sleep(500);
        system("cls");
        std::cout << "Chatting with " << targetUser.getUsername() << ". Type 'exit' to end the chat.\n" << std::endl;
        displayMessages();

        std::string input;
        std::cout << loggedInUser.getUsername() << ": ";
        std::getline(std::cin, input);

        if (input == "exit") {
            return;
        }
        else if(!input.empty()){
            sendMessage(input);
        }
    }
}

void Chat::sendMessage(const std::string& text) const {
    // Insert the new message into the messages table
    std::string insertMessageQuery =
        "INSERT INTO messages (chat_id, user_id, message_text) VALUES ("
        + std::to_string(loggedInUser.getDefaultChatId(dbConnection,targetUser)) + ", '"
        + std::to_string(loggedInUser.getId()) + "', '" + text + "')";

    if (dbConnection.executeQuery(insertMessageQuery)) {
        std::cout << "Message sent to " << targetUser.getUsername() << ": " << text << std::endl;
    }
    else {
        std::cerr << "Failed to send message." << std::endl;
    }
}
void Chat::displayMessages() const {
    std::vector<Message> messages = getMessages();

    for (const auto& message : messages) {
        message.display();
    }
}

std::vector<Message> Chat::getMessages() const {
    std::vector<Message> messages;
    
    // Retrieve and store chat history with sender name, message, and timestamp
    std::string selectMessagesQuery =
        "SELECT m.user_id, u.username, m.message_text, m.timestamp "
        "FROM messages m "
        "JOIN users u ON m.user_id = u.user_id "
        "WHERE m.chat_id = " + std::to_string(loggedInUser.getDefaultChatId(dbConnection, targetUser)) +
        " ORDER BY m.timestamp ASC";

    MYSQL_RES* result = nullptr;

    if (dbConnection.executeQuery(selectMessagesQuery)) {
        result = mysql_store_result(dbConnection.getConnection());

        if (result != nullptr) {
            // Clear messages before populating it with new data
            messages.clear();

            while (MYSQL_ROW row = mysql_fetch_row(result)) {
                // Ensure that the result columns are not null before accessing
                if (row[0] != nullptr && row[1] != nullptr && row[2] != nullptr && row[3] != nullptr) {
                    int userId = std::stoi(row[0]);
                    std::string senderName = row[1];
                    std::string messageText = row[2];
                    std::string timestamp = row[3];

                    messages.emplace_back(userId, senderName, messageText, timestamp);
                }
                else {
                    // Handle the case where a column is unexpectedly null
                    std::cerr << "Unexpected null value in the result set." << std::endl;
                }
            }

            mysql_free_result(result);
        }
    }


    return messages;
}