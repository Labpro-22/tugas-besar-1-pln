#pragma once

#include <list>
#include <queue>
#include <vector>

#include "models/Player.hpp"
#include "models/board/Board.hpp"
#include "models/card/Card.hpp"
#include "models/card/CardDeck.hpp"
#include "models/card/chance-card/ChanceCard.hpp"
#include "models/card/community-chest-card/CommunityChestCard.hpp"
#include "models/card/skill-card/SkillCard.hpp"
#include "models/property/Property.hpp"
#include "models/tile/Tile.hpp"
#include "utils/config/Config.hpp"
#include "utils/config/ConfigLoader.hpp"
#include "utils/log/TransactionLogger.hpp"
#include "views/GameView.hpp"

template <typename T>
class CardDeck<T>;

class Bank;

class GameManager {
private:
    bool isRunning;
    Config config;
    GameView gameView;
    int turn;
    Board *board;
    std::vector<Player *> players;
    Bank bank;
    TransactionLogger logger;
    CardDeck<ChanceCard> chanceCardDeck;
    CardDeck<CommunityChestCard> communityCardDeck;
    CardDeck<SkillCard> skillCardDeck;
    void gameLoop();

public:
    GameManager() : config{ConfigLoader::loadConfig("config/config.txt")}, gameView{this}, board{nullptr}
    {
    }
    ~GameManager()
    {
        if (board != nullptr) {
            delete board;
            board = nullptr;
        }
    }

    const Config &getConfig() const;
    int getCurrentTurn() const;
    Player *getCurrentPlayer() const;
    Board *getBoard() const;
    Bank *getBank() const;
    const std::vector<Player *> getPlayers() const;
    TransactionLogger *getLogger() const;

    void runGame();
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
};