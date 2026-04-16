#pragma once

#include "ChanceCard.hpp"

class ForcedMoveCard : public ChanceCard {
private:
    int moveOffset;

public:
    ForcedMoveCard(const int moveOffset);
    void takeEffect(Player& player) override;
}