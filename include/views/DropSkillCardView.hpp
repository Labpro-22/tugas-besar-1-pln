#pragma once
#include "../core/GameManager.hpp"

class DropSkillCardView
{
private:
    GameManager& gameManager;
public:
    DropSkillCardView(GameManager& gm) : gameManager(gm) {}
    int promptChooseSkillCard(std::vector<SkillCard*> sc);
    void outputDropSkillCardStatus(bool success);
};

