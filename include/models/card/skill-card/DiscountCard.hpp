#pragma once
#include "SkillCard.hpp"

class DiscountCard : public SkillCard {
private:
    int percentage;

public:
    explicit DiscountCard(const std::string& message);
    void takeEffect(Player& p) override;
    void rerandomizePercentage();
};