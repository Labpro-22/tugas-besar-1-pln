#pragma once
#include "SkillCard.hpp"

class ShieldCard : public SkillCard {
public:
    explicit ShieldCard(const std::string& message);
    void takeEffect(Player& p) override;
};