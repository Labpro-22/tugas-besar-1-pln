#pragma once
#include "models/card/skill-card/SkillCard.hpp"

class TeleportCard : public SkillCard {
public:
    explicit TeleportCard(const std::string& message);
    void void takeEffect(Player& p,GameManager& gm) override;
    void setTargetPosition(int pos) { targetPosition = pos; }
};