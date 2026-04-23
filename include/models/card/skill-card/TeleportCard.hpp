#pragma once
#include "models/card/skill-card/SkillCard.hpp"

class TeleportCard : public SkillCard {
private:
    int targetPosition = -1;

public:
    TeleportCard(const std::string &message);
    void setTargetPosition(int pos);
    void takeEffect(Player &p, GameManager &gm) override;
    void prepareUse(UseSkillCardView &view, GameManager &gm) override;
    std::string getCardType() const override;
};