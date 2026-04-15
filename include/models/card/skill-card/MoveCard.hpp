#pragma once
#include "SkillCard.hpp"

class MoveCard : public SkillCard {
private:
    int amount;

public:
    explicit MoveCard(const std::string& message);
    void takeEffect(Player& p) override;
    void rerandomizeAmount();
};