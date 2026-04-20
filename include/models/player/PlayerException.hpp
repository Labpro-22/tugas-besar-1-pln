#pragma once

#include <exception>
#include <string>

class PlayerException : public std::exception {
private:
    std::string errorMessage;

public:
    PlayerException(const std::string& errorMessage) : errorMessage(errorMessage) {}

    const char* what() const noexcept override {
        return errorMessage.c_str();
    }
};

class InvalidCardIndexException : public PlayerException {
public:
    InvalidCardIndexException(const std::string& msg) : PlayerException(msg) {}
};

class CantBankruptException : public PlayerException {
public:
    CantBankruptException(const std::string& msg) : PlayerException(msg) {}
};

class AlreadyBankruptException : public PlayerException {
public:
    AlreadyBankruptException(const std::string& msg) : PlayerException(msg) {}
};

class InJailException : public PlayerException {
public:
    InJailException(const std::string& msg) : PlayerException(msg) {}
};

class InsufficientFundsException : public PlayerException {
public:
    InsufficientFundsException(const std::string& msg) : PlayerException(msg) {}
};

class FullHandException : public PlayerException {
public:
    FullHandException(const std::string& msg) : PlayerException(msg) {}
};

