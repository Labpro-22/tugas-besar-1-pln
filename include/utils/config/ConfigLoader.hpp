#pragma once

#include <filesystem>

#include "utils/config/Config.hpp"

class ConfigLoader {
private:
    ConfigLoader() = default;

public:
    static Config loadConfig(std::filesystem::path path);
};