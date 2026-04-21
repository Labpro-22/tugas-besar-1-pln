#pragma once
class GameManager;

class MainMenuView
{
private:
    GameManager& gameManager;
public:
    MainMenuView(GameManager& gm) : gameManager(gm) {}
    int promptNewOrLoadGame() const;
};

