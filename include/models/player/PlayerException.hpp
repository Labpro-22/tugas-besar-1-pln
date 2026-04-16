#pragma once

#include <exception>
#include <string>

using namespace std;

class PlayerException : public exception {
private:
    string errorMessage;

public:
    PlayerException(const string& errorMessage) : errorMessage(errorMessage) {}

    const char* what() const noexcept override {
        return errorMessage.c_str();
    }
};

class InvalidCardIndexException : public PlayerException {
public:
    InvalidCardIndexException(const string& msg) : PlayerException(msg) {}
};

class CantBankruptException : public PlayerException {
public:
    CantBankruptException(const string& msg) : PlayerException(msg) {}
};

class AlreadyBankruptException : public PlayerException {
public:
    AlreadyBankruptException(const string& msg) : PlayerException(msg) {}
};

class InJailException : public PlayerException {
public:
    InJailException(const string& msg) : PlayerException(msg) {}
};

class InsufficientFundsException : public PlayerException {
public:
    InsufficientFundsException(const string& msg) : PlayerException(msg) {}
};

class FullHandException : public PlayerException {
public:
    FullHandException(const string& msg) : PlayerException(msg) {}
};

