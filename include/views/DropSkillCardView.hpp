#pragma once
#include "models/card/skill-card/SkillCard.hpp"
#include <vector>
class GameManager;

class DropSkillCardView
{
private:
    GameManager& gameManager;
public:
    DropSkillCardView(GameManager& gm) : gameManager(gm) {}
    SkillCard* promptChooseSkillCard(std::vector<SkillCard*> sc) const;
    void outputDropSkillCardStatus(SkillCard &card) const;
};

