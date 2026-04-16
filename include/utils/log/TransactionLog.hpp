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
    std::string toString();
};