#include "core/GameManager.hpp"

// Main
void runGame();
bool GameManager::isGameEnded()
{
    return !isRunning() || playerLeaderboard.size() == players.size() - 1 || turn >= config.maxTurn;
}

void GameManager::gameLoop()
{
    if (isGameEnded()) {
    }
    if (playerQueue.empty()) {
    }
}

// Getters
const Config &GameManager::getConfig() const { return config; }
int GameManager::getCurrentTurn() const { return turn; }
Player *GameManager::getCurrentPlayer() const { return playerQueue.front(); }
Board *GameManager::getBoard() const { return board; }
Bank *GameManager::getBank() const { return bank; }
const std::vector<Player *> GameManager::getPlayers() const { return players; }
TransactionLogger *GameManager::getLogger() const { return logger; }

// Game action
void processMainMenu();
void processNewGame();
void processLoadGame();
void processSaveGame();
void startNextTurn();
void processLoadGame();
void processRollDice();
void processSetDice(int value1, int value2);
void processBuyProperty();
void processMortgageProperty();
void processUnmortgageProperty();
void processBuild();
void processUseSkillCard();
void processDropSkillCard();
void processLiquidation();
void processPrintLogs();