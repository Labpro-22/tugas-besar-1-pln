#pragma once

#include <filesystem>

#include "utils/config/Config.hpp"

class ConfigLoader
{
public:
    static Config loadConfig(std::filesystem::path path);
};