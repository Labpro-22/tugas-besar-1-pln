#pragma once

#include <filesystem>

#include "utils/save-load/SaveData.hpp"

class SaveFileHandler {
public:
    static void saveGame(SaveData saveData, std::filesystem::path path);
    static SaveData loadGame(std::filesystem::path path);
};