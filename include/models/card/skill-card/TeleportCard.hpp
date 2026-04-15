#pragma once
#include "SkillCard.hpp"

class TeleportCard : public SkillCard {
public:
    explicit TeleportCard(const std::string& message);
    void takeEffect(Player& p) override;
};