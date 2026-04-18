#pragma once
#include "models/card/skill-card/SkillCard.hpp"

class LassoCard : public SkillCard {
public:
    explicit LassoCard(const std::string& message);
    void takeEffect(Player& p,GameManager& gm) override;
};