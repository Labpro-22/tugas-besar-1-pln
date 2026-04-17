#pragma once
#include "core/GameManager.hpp"

class DropSkillCardView
{
private:
    GameManager& gameManager;
public:
    DropSkillCardView(GameManager& gm) : gameManager(gm) {}
    int promptChooseSkillCard(std::vector<SkillCard*> sc) const;
    void outputDropSkillCardStatus(bool success) const;
};

