#pragma once

#include <string>

class PropertyConfig {
public:
    int id;
    std::string code;
    std::string name;
    std::string color;
    long long price;
    long long mortgageValue;
    long long rent[6];
};