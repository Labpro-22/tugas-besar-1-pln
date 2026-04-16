#include "models/bank/Bank.hpp"

void Bank::giveMoney(Player &player, long long amount)
{
    player.getMoney(amount);
}

void Bank::giveMoneyToAll(std::vector<Player> &players, long long amount)
{
    for (Player &player : players) {
        player.getMoney(amount);
    }
}

void Bank::collectMoney(Player &player, long long amount)
{
    player.payTax(amount);
}

void Bank::collectMoneyFromAll(std::vector<Player> &players, long long amount)
{
    for (Player &player : players) {
        player.payTax(amount);
    }
}

void Bank::startAuction(Property &property) {
    
}