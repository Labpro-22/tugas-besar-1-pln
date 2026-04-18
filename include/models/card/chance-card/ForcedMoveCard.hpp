#pragma once

#include "models/card/chance-card/ChanceCard.hpp"

class ForcedMoveCard : public ChanceCard {
private:
    int moveOffset;

public:
    ForcedMoveCard(const int moveOffset);
    void takeEffect(Player& p,GameManager& gm) override;
};