#pragma once
class GameManager;

class MainMenuView
{
private:
    GameManager& gameManager;
public:
    MainMenuView(GameManager& gm) : gameManager(gm) {}
    int promptNewGameOrLoadGame() ;
    std::vector<std::string> promptUsernames();
};

