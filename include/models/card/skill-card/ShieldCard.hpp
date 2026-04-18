#pragma once
#include "models/card/skill-card/SkillCard.hpp"

class ShieldCard : public SkillCard {
public:
    explicit ShieldCard(const std::string& message);
    void takeEffect(Player& p,GameManager& gm) override;
};