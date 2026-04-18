#pragma once

#include <string>

class PropertyConfig {
public:
    int id;
    std::string code;
    std::string name;
    std::string color;
    std::string type;
    long long price;
    long long mortgageValue;
    long long housePrice;
    long long hotelPrice;
    long long rent[6];
};