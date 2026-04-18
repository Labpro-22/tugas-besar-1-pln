#pragma once
#include <string>
#include "models/Player.hpp"
#include "models/GameManager.hpp"

class Card {
private:
    std::string message;

public:
    Card(const std::string& message);
    virtual ~Card() = default;
    virtual void takeEffect(Player& p,GameManager& gm) = 0;
    std::string getMessage();
};