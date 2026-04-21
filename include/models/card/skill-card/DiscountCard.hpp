#pragma once
#include "models/card/skill-card/SkillCard.hpp"

class DiscountCard : public SkillCard {
private:
    int percentage;
    
public:
    DiscountCard(const std::string& message, int percentage);
    void takeEffect(Player& p, GameManager& gm) override;
    std::string getType() override;
};