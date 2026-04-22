#pragma once

#include <filesystem>

#include "utils/config/Config.hpp"

class ConfigLoader {
private:
    ConfigLoader() = default;
    static void loadActionTile(Config &config, std::filesystem::path path);
    static void loadProperty(Config &config, std::filesystem::path path);
    static void loadRailroad(Config &config, std::filesystem::path path);
    static void loadUtility(Config &config, std::filesystem::path path);
    static void loadTax(Config &config, std::filesystem::path path);
    static void loadSpecial(Config &config, std::filesystem::path path);
    static void loadMisc(Config &config, std::filesystem::path path);

public:
    static Config loadConfig(std::string path = "default/");
};