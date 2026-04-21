#pragma once

#include <filesystem>
#include <fstream>

#include "utils/save-load/SaveData.hpp"
#include "utils/save-load/SaveFileException.hpp"

class SaveFileHandler {
private:
    SaveFileHandler() = default;

    static void savePlayer(SaveData &saveData, std::ofstream &out);
    static void saveProperty(SaveData &saveData, std::ofstream &out);
    static void saveDeck(SaveData &saveData, std::ofstream &out);
    static void saveLog(SaveData &saveData, std::ofstream &out);

    static void loadPlayer(SaveData &saveData, std::ifstream &in, std::filesystem::path &path, int &line);
    static void loadProperty(SaveData &saveData, std::ifstream &in, std::filesystem::path &path, int &line);
    static void loadDeck(SaveData &saveData, std::ifstream &in, std::filesystem::path &path, int &line);
    static void loadLog(SaveData &saveData, std::ifstream &in, std::filesystem::path &path, int &line);

public:
    static void saveGame(SaveData saveData, std::filesystem::path path);
    static SaveData loadGame(std::filesystem::path path);
};