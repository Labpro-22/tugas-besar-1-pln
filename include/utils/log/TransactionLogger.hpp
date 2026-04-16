#pragma once

#include <vector>

#include "utils/log/TransactionLog.hpp"

class TransactionLogger
{
private:
    std::vector<TransactionLog> logs;
public:
    void log(int turn, std::string username, std::string action, std::string details);
    const std::vector<TransactionLog>& getLogs() const;
    void clear();
};