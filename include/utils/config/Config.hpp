#pragma once

#include <map>
#include <vector>

#include "utils/config/ActionConfig.hpp"
#include "utils/config/CardConfig.hpp"
#include "utils/config/PropertyConfig.hpp"

class Config {
public:
    int maxTurn;
    int playerCount;
    std::map<int, std::string> tileCodeSequence;
    std::vector<ActionTileConfig> actionTiles;
    std::vector<PropertyConfig> properties;
    std::map<int, long long> railroadRent;
    std::map<int, long long> utilityRent;
    long long incomeFlatTax;
    long long incomePercentageTax;
    long long luxuryFlatTax;
    long long goSalary;
    long long jailFine;
    long long initialMoney;
    std::vector<CardConfig> chanceCards;
    std::vector<CardConfig> communityChestCards;
};