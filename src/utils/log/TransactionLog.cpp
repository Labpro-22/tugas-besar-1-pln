#include "utils/log/TransactionLog.hpp"

TransactionLog::TransactionLog(int turn, std::string username, std::string action, std::string details)
    : turn{turn}, username{std::move(username)}, action{std::move(action)}, details{std::move(details)} {}

std::string TransactionLog::toString()
{
    return std::to_string(turn) + " " + username + " " + action + " " + details;
}