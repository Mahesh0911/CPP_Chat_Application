// User.cpp
#include "User.h"

User::User()
    : userId(-1), username("\0"), password("\0") {}

User::User(int userId, const std::string& username, const std::string& password)
    : userId(userId), username(username), password(password) {}

int User::getId() const {
    return userId;
}

std::string User::getUsername() const {
    return username;
}

std::string User::getPassword() const {
    return password;
}

int User::getDefaultChatId(DatabaseConnection& dbConnection, const User& targetUser) const {
    // Retrieve the chat_id from the chats table based on the user_ids
    std::string selectChatIdQuery =
        "SELECT chat_id FROM chats "
        "WHERE (user_id1 = " + std::to_string(userId) + " AND user_id2 = " + std::to_string(targetUser.getId()) + ") "
        "   OR (user_id1 = " + std::to_string(targetUser.getId()) + " AND user_id2 = " + std::to_string(userId) + ")";

    MYSQL_RES* result = nullptr;

    if (dbConnection.executeQuery(selectChatIdQuery)) {
        result = mysql_store_result(dbConnection.getConnection());

        if (result != nullptr && mysql_num_rows(result) > 0) {
            MYSQL_ROW row = mysql_fetch_row(result);
            int chatId = std::stoi(row[0]);
            if (result != nullptr) {
                mysql_free_result(result);
            }
            return chatId;
        }
    }

    // If no existing chat, create a new chat and return its chat_id
    return createNewChat(dbConnection, targetUser);
}

int User::createNewChat(DatabaseConnection& dbConnection, const User& targetUser) const {
    // Insert a new row into the chats table to create a new chat
    std::string insertChatQuery =
        "INSERT INTO chats (user_id1, user_id2) VALUES (" + std::to_string(userId) + ", " + std::to_string(targetUser.getId()) + ")";

    if (dbConnection.executeQuery(insertChatQuery)) {
        return mysql_insert_id(dbConnection.getConnection()); // Return the chat_id of the newly created chat
    }

    return -1; // Return -1 if there is an error
}

std::vector<User> User::getPreviousChatUsers(DatabaseConnection& dbConnection) const {
    std::vector<User> previousChatUsers;

    // Retrieve users the logged-in user has chatted with before
    std::string selectPreviousChatUsersQuery =
        "SELECT DISTINCT u.user_id, u.username "
        "FROM users u "
        "JOIN chats c ON (u.user_id = c.user_id1 OR u.user_id = c.user_id2) "
        "WHERE (c.user_id1 = " + std::to_string(userId) + " OR c.user_id2 = " + std::to_string(userId) + ") "
        "AND u.user_id != " + std::to_string(userId);

    MYSQL_RES* result = nullptr;

    if (dbConnection.executeQuery(selectPreviousChatUsersQuery)) {
        result = mysql_store_result(dbConnection.getConnection());

        if (result != nullptr) {
            // Clear previousChatUsers before populating it with new data
            previousChatUsers.clear();

            while (MYSQL_ROW row = mysql_fetch_row(result)) {
                // Ensure that the result columns are not null before accessing
                if (row[0] != nullptr && row[1] != nullptr) {
                    int userId = std::stoi(row[0]);
                    std::string username = row[1];
                    previousChatUsers.push_back(User(userId, username, ""));  // Use an empty string
                }
            }

            mysql_free_result(result);
        }
    }


    return previousChatUsers;
}

User User::getUserByUsername(DatabaseConnection& dbConnection, const std::string& targetUsername) {
    std::string selectUserQuery = "SELECT user_id, username FROM users WHERE username='" + targetUsername + "'";
    MYSQL_RES* result = nullptr;

    if (dbConnection.executeQuery(selectUserQuery)) {
        result = mysql_store_result(dbConnection.getConnection());

        if (result != nullptr && mysql_num_rows(result) > 0) {
            MYSQL_ROW row = mysql_fetch_row(result);
            int userId = std::stoi(row[0]);
            std::string username = row[1];
            mysql_free_result(result);
            return User(userId, username,"\0");
        }
    }

    return User(); // Return an empty User object if not found
}