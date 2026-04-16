#pragma once

#include <vector>

#include "utils/config/PropertyConfig.hpp"

class Config {
public:
    int maxTurn;
    int playerCount;
    std::vector<PropertyConfig> properties;
    long long railroadRent;
    long long utilityRent;
    long long incomeFlatTax;
    long long incomePercentageTax;
    long long luxuryTax;
    long long goSalary;
    long long jailFine;
    long long initialMoney;
    std::vector<int> tileSequence;
};