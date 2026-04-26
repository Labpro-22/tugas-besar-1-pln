#pragma once

#include "models/card/chance-card/ChanceCard.hpp"

class ForcedMoveCard : public ChanceCard {
private:
    int moveOffset;

public:
    ForcedMoveCard(const std::string& message, int moveOffset);
    void takeEffect(Player& p, GameManager& gm) override;
    void generateForcedMoveMessage(int offset);
    std::string getCardType() const override;
};