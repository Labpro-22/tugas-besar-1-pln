#pragma once
#include "models/card/skill-card/SkillCard.hpp"

class LassoCard : public SkillCard {
private:
    Player* targetPlayer = nullptr;
public:
    LassoCard(const std::string& message);
    void setTargetPlayer(Player* p);
    void takeEffect(Player& p, GameManager& gm) override;
};