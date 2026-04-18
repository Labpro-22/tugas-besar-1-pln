#pragma once

#include "models/card/chance-card/ChanceCard.hpp"

class GoToNearestStationCard : public ChanceCard {
public:
    GoToNearestStationCard(const std::string& message) : ChanceCard(message) {}
    void takeEffect(Player& p,GameManager& gm) override;
};