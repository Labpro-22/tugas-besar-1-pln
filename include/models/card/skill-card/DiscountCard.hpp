#pragma once
#include "models/card/skill-card/SkillCard.hpp"

class DiscountCard : public SkillCard {
private:
    int percentage;

public:
    explicit DiscountCard(const std::string& message);
    void takeEffect(Player& p,GameManager& gm) override;
    void rerandomizePercentage();
};