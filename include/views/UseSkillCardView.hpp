#pragma once
#include "core/GameManager.hpp"

class UseSkillCardView
{
private:
    GameManager& gameManager;
public:
    int promptChooseCardToUse(const std::vector<SkillCard*>& cards);
    void processAndPlayCard(SkillCard* card, Player& player, GameManager& gm);

    int askForTargetPosition();
    Player* askForTargetPlayer(GameManager& gm);
    int askForDemolitionTileId();   
};
