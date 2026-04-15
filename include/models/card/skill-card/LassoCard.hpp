#pragma once
#include "SkillCard.hpp"

class LassoCard : public SkillCard {
public:
    explicit LassoCard(const std::string& message);
    void takeEffect(Player& p) override;
};