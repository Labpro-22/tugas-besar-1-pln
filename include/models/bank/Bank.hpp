#pragma once

#include <vector>

#include "models/player/Player.hpp"

class Bank {
public:
    void giveInitialMoney(std::vector<Player> &players);
    void giveMoney(Player &player, long long amount);
    void giveMoneyToAll(std::vector<Player> &players, long long amount);
    void collectMoney(Player &player, long long amount);
    void collectMoneyFromAll(std::vector<Player> &players, long long amount);
    void startAuction(Property &property);
};