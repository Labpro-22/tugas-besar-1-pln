#pragma once
#include <string>

class GameManager;
class Player;
class JailView
{
private:
    GameManager& gameManager;
public:
    JailView(GameManager &gm) : gameManager(gm){}
    void outputGoToJail(const std::string& reason = "") const;
    void outputGoToJail(Player &player, const std::string& reason = "") const;
    int promptRollOrBailOrUseCard();
};
