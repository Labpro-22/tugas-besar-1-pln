#pragma once
#include <exception>
#include <string>

class CardDeckException : public std::exception {
private:
    std::string errorMessage;
public:
    CardDeckException(const std::string& errorMessage) : errorMessage(errorMessage) {}

    const char* what() const noexcept override {
        return errorMessage.c_str();
    }
};