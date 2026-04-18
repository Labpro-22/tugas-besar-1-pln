#pragma once

#include <stdexcept>

class ConfigException : std::exception {
private:
    std::string errorMessage;

public:
    ConfigException(const std::string &errorMessage) : errorMessage("[ConfigLoader] " + errorMessage) {}

    const char *what() const noexcept override
    {
        return errorMessage.c_str();
    }
};

class FileNotFoundException : ConfigException {
public:
    FileNotFoundException(const std::string &path) : ConfigException("File tidak ditemukan pada: " + path) {}
};

class InputFormatException : ConfigException {
public:
    InputFormatException(const std::string &expected, const std::string &path, int col, int line)
        : ConfigException("Error format input. Ekspektasi " + expected + " sebagai input ke-" + std::to_string(col) +
                          ". File: " + path + " baris ke-" + std::to_string(line) + ".") {}
};