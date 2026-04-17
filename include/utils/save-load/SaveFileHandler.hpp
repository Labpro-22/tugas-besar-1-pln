#pragma once

#include <filesystem>

#include "utils/save-load/SaveData.hpp"

class SaveFileHandler {
private:
    SaveFileHandler() = default;

public:
    static void saveGame(SaveData saveData, std::filesystem::path path);
    static SaveData loadGame(std::filesystem::path path);
};