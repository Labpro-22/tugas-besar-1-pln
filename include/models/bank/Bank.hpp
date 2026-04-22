#pragma once

#include <vector>

#include "models/player/Player.hpp"
#include "models/property/Property.hpp"
#include "models/property/StreetProperty.hpp"
#include "models/property/RailroadProperty.hpp"
#include "models/property/UtilityProperty.hpp"
#include "utils/save-load/PropertySaveData.hpp"

class Bank {
private:
    long long initialMoney;
    std::vector<Property> properties;

public:
    Bank(long long initialMoney, Config &config);
    Bank(long long initialMoney, Config &config, std::vector<PropertySaveData> &saveData, std::vector<Player>& players);

    std::vector<Property>& getProperties();

    void giveInitialMoney(std::vector<Player> &players);
    void giveMoney(Player &player, long long amount);
    void giveMoneyToAll(std::vector<Player> &players, long long amount);
    void collectMoney(Player &player, long long amount);
    void collectMoneyFromAll(std::vector<Player> &players, long long amount);
};