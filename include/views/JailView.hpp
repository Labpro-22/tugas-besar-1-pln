#pragma once
#include <string>

class GameManager;
class JailView
{
private:
    GameManager& gameManager;
public:
    JailView(GameManager &gm) : gameManager(gm){}
    void outputGoToJail(const std::string& reason = "") const;
    int promptRollOrBailOrUseCard();
};
