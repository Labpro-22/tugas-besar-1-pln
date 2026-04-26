#pragma once

#include <exception>
#include <string>

class SaveFileException : std::exception {
private:
    std::string errorMessage;

public:
    SaveFileException(const std::string &errorMessage) : errorMessage("[SaveFileHandler] " + errorMessage) {}

    const char *what() const noexcept override
    {
        return errorMessage.c_str();
    }
};

class SaveFileNotFoundException : public SaveFileException {
public:
    SaveFileNotFoundException(const std::string &path) : SaveFileException("File tidak ditemukan pada: " + path) {}
};

class SaveFileFormatException : public SaveFileException {
public:
    SaveFileFormatException(const std::string &expected, const std::string &path, int col, int line)
        : SaveFileException("Error format input. Ekspektasi " + expected + " sebagai input ke-" + std::to_string(col) +
                            ". File: " + path + " baris ke-" + std::to_string(line) + ".") {}
};