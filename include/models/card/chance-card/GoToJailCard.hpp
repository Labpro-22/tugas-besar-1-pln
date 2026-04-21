#pragma once

#include "models/card/chance-card/ChanceCard.hpp"

class GoToJailCard : public ChanceCard {
public:
    GoToJailCard(const std::string& message);
    void takeEffect(Player& p, GameManager& gm) override;
};