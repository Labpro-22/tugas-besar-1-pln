#pragma once
#include "models/card/skill-card/SkillCard.hpp"

class MoveCard : public SkillCard {
private:
    int amount;

public:
    explicit MoveCard(const std::string& message);
    void takeEffect(Player& p,GameManager& gm) override;
    void rerandomizeAmount();
};