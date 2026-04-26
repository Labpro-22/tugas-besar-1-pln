#pragma once

#include <string>

class PropertySaveData {
public:
    std::string code;
    std::string type;
    std::string owner;
    std::string status;
    long long festivalMultiplier;
    int festivalDuration;
    int houseCount;
    bool hasHotel;
};