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
    int promptChooseSkillCard(const std::vector<SkillCard*>& card);
    void outputDropSkillCardStatus(SkillCard &card);
};

