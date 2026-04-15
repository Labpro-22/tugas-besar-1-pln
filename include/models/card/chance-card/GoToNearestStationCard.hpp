#pragma once

#include "ChanceCard.hpp"

class GoToNearestStationCard : public ChanceCard {
public:
    GoToNearestStationCard() {};
    void takeEffect(Player& player);
};