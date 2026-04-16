#include <algorithm>
#include <random>

#include "core/GameManager.hpp"

GameManager::GameManager()
{
}

// Game runner
void GameManager::runGame()
{
    running = true;
    while (running) {
        gameLoop();
    }
}

void GameManager::stopGame()
{
    running = false;
}
bool GameManager::isGameEnded()
{
    return playerLeaderboard.size() == players.size() - 1 || turn >= config.maxTurn;
}

void GameManager::startGame()
{
    running = false;
}

void GameManager::gameLoop()
{
    if (inMainMenu) {
        processMainMenu();
    }
    else {
        if (isGameEnded()) {
            processWin();
        }
        else {
            gameView.nextCommand();
        }
    }
}

// Getters
const Config &GameManager::getConfig() const { return config; }
int GameManager::getCurrentTurn() const { return turn; }
Player &GameManager::getCurrentPlayer() const { return playerQueue.front(); }
Board &GameManager::getBoard() const { return board; }
Bank &GameManager::getBank() const { return bank; }
std::vector<Player>& GameManager::getPlayers() const { return players; }
TransactionLogger &GameManager::getLogger() const { return logger; }

// Game action
void GameManager::processMainMenu()
{
    MainMenuView mainMenuView = gameView.getMainMenuView();
    mainMenuView.promptNewGameOrLoadGame();
}
void GameManager::processNewGame()
{
    MainMenuView mainMenuView = gameView.getMainMenuView();

    // Create players
    std::vector<std::string> usernames = mainMenuView.promptUsernames();
    players.clear();
    for (int i = 0; i < usernames.size(); i++) {
        players.push_back(Player{usernames[i]});
    }
    std::shuffle(players.begin(), players.end(), std::default_random_engine{time(0)});

    std::vector<int> tileID = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40};

    // Create board
    board = Board(40, tileID, config.properties, players);

    // Clear logger
    logger.clear();

    inMainMenu = false;
    startGame();
    startNextTurn();
}
void GameManager::processLoadGame();
void GameManager::processSaveGame();
void GameManager::startNextTurn();
void GameManager::processLoadGame();
void GameManager::processRollDice();
void GameManager::processSetDice(int value1, int value2);
void GameManager::processBuyProperty();
void GameManager::processMortgageProperty();
void GameManager::processUnmortgageProperty();
void GameManager::processBuild();
void GameManager::processUseSkillCard();
void GameManager::processDropSkillCard();
void GameManager::processLiquidation();
void GameManager::processPrintLogs();