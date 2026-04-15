#pragma once
#include "../Card.hpp"

// TODO : message attribute
class ChanceCard : public Card {
private:
    std::string message;
public:
    ChanceCard(const std::string& message);
    virtual ~ChanceCard() = default;
    virtual takeEffect(Player& player) = 0;
};