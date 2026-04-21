#pragma once
#include <vector>

class SkillCard;
class Player;
class GameManager;

class UseSkillCardView {
public:
    int promptChooseCardToUse(const std::vector<SkillCard*>& cards);
    void processAndPlayCard(SkillCard* card, Player& player, GameManager& gm);

    int askForTargetPosition();
    Player* askForTargetPlayer(GameManager& gm);
    int askForDemolitionTileId();   
};