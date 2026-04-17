#include "models/bank/Bank.hpp"

Bank::Bank(long long initialMoney) : initialMoney{initialMoney} {}


void Bank::giveInitialMoney(std::vector<Player> &players)
{
    giveMoneyToAll(players, initialMoney);
}

void Bank::giveMoney(Player &player, long long amount)
{
    player.receiveMoney(amount);
}

void Bank::giveMoneyToAll(std::vector<Player> &players, long long amount)
{
    for (Player &player : players) {
        player.receiveMoney(amount);
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

void Bank::startAuction(Property &property)
{
}