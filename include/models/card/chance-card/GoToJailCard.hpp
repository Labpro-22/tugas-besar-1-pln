#pragma once

#include "ChanceCard.hpp"

class GoToJailCard : public ChanceCard {
public:
    GoToJailCard(const std::string& message) : message(message) {}
    void takeEffect(Player& player) override;
};