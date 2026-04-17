#pragma once

#include <string>
#include <vector>

#include "utils/save-load/LogSaveData.hpp"
#include "utils/save-load/PlayerSaveData.hpp"
#include "utils/save-load/PropertySaveData.hpp"

class SaveData {
public:
    int turn;
    int maxTurn;
    std::vector<PlayerSaveData> players;
    std::vector<PropertySaveData> properties;
    std::vector<std::string> deckCards;
    std::vector<LogSaveData> logs;
};