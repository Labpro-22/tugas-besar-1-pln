#pragma once
class GameManager;
class JailView
{
private:
    GameManager& gameManager;
public:
    JailView(GameManager &gm) : gameManager(gm){}
    void outputGoToJail() const;
    int promptRollOrBailOrUseCard();
};
