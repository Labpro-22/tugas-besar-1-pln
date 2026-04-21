#pragma once

#include <string>

class TransactionLog {
private:
    int turn;
    std::string username;
    std::string action;
    std::string details;

public:
    TransactionLog(int turn, std::string username, std::string action, std::string details);

    int getTurn() const;
    std::string getUsername() const;
    std::string getAction() const;
    std::string getDetails() const;

    std::string toString() const;
};