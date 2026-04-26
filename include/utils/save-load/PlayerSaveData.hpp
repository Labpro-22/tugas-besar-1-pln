#pragma once

#include <string>
#include <vector>

#include "utils/save-load/SkillCardSaveData.hpp"

class PlayerSaveData {
public:
    std::string username;
    long long money;
    std::string tileCodePosition;
    std::string status;
    std::vector<SkillCardSaveData> skillCards;
    int getOutOfJailCardCount;
    int jailTurns;
};
