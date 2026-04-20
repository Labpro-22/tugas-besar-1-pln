#pragma once

#include "models/card/chance-card/ChanceCard.hpp"

class GetOutOfJailCard : public ChanceCard {
public:
    GetOutOfJailCard(const std::string& message);
    void takeEffect(Player& p, GameManager& gm) override;
};