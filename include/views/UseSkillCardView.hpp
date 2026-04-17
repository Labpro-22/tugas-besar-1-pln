#pragma once
#include "core/GameManager.hpp"

class UseSkillCardView
{
private:
    GameManager& gameManager;
public:
    UseSkillCardView(GameManager& gm) : gameManager(gm) {}
    int promptChooseSkillCard(std::vector<SkillCard*> sc) const;
    void outputUseSkillCardStatus(bool success) const;
};

