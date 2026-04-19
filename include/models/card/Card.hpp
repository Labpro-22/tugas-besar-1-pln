#pragma once
#include <string>
#include "models/player/Player.hpp"

class Card {
private:
    std::string message;

public:
    Card(const std::string& message);
    virtual ~Card() = default;
    virtual void takeEffect(Player& p) = 0;
    std::string getMessage();
};