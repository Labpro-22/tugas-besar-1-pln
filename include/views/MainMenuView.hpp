#pragma once
class GameManager;

class MainMenuView
{
private:
    GameManager& gameManager;
public:
    const std::string RESET = "\033[0m";
    const std::string BOLD = "\033[1m";
    const std::string CYAN = "\033[36m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string RED = "\033[31m";
    
    MainMenuView(GameManager& gm) : gameManager(gm) {}
    int promptNewGameOrLoadGame() ;
    std::vector<std::string> promptUsernames();
    void outputCurrentPlayer();
};

