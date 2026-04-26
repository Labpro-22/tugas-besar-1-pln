#include "utils/log/TransactionLogger.hpp"

void TransactionLogger::log(int turn, std::string username, std::string action, std::string details)
{
    logs.push_back(TransactionLog{turn, username, action, details});
}

const std::vector<TransactionLog> &TransactionLogger::getLogs() const
{
    return logs;
}

void TransactionLogger::clear() {
    logs.clear();
}

