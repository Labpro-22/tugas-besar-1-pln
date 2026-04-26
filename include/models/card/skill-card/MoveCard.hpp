#pragma once
#include "models/card/skill-card/SkillCard.hpp"

class MoveCard : public SkillCard {
private:
    int amount;

public:
    MoveCard(const std::string &message, int amount);
    int getAmount() const;
    void takeEffect(Player &p, GameManager &gm) override;
    std::string getCardType() const override;
    int getCardValue() const override { return amount; }
};