#include "utils/log/TransactionLog.hpp"

TransactionLog::TransactionLog(int turn, std::string username, std::string action, std::string details)
    : turn{turn}, username{std::move(username)}, action{std::move(action)}, details{std::move(details)} {}

int TransactionLog::getTurn() const{ return turn; }
std::string TransactionLog::getUsername() const{ return username; }
std::string TransactionLog::getAction() const{ return action; }
std::string TransactionLog::getDetails()const { return details; }

std::string TransactionLog::toString() const
{
    return std::to_string(turn) + " " + username + " " + action + " " + details;
}