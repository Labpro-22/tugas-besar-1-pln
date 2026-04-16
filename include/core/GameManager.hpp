#pragma once

#include <queue>
#include <vector>

#include "models/bank/Bank.hpp"
#include "models/board/Board.hpp"
#include "models/card/Card.hpp"
#include "models/card/CardDeck.hpp"
#include "models/card/chance-card/ChanceCard.hpp"
#include "models/card/community-chest-card/CommunityChestCard.hpp"
#include "models/card/skill-card/SkillCard.hpp"
#include "models/player/Player.hpp"
#include "models/property/Property.hpp"
#include "models/tile/Tile.hpp"
#include "utils/config/Config.hpp"
#include "utils/config/ConfigLoader.hpp"
#include "utils/log/TransactionLogger.hpp"
#include "views/GameView.hpp"

template <typename T>
class CardDeck<T>;

class GameManager {
private:
    bool running;
    bool inMainMenu;
    Config config;
    GameView gameView;
    int turn;
    Board board;
    std::vector<Player> players;
    std::vector<Player *> playerLeaderboard;
    std::queue<Player *> playerQueue;
    Bank bank;
    TransactionLogger logger;
    CardDeck<ChanceCard> chanceCardDeck;
    CardDeck<CommunityChestCard> communityCardDeck;
    CardDeck<SkillCard> skillCardDeck;

    // main game runner
    void gameLoop();
    bool isRunning() const;
    bool isGameEnded() const;
    void nextTurn();
    void nextPlayer();

    void releaseResource();

public:
    GameManager()
        : running{false},
          inMainMenu{false},
          config{ConfigLoader::loadConfig("config/config.txt")},
          gameView{this},
          board{nullptr}
    {
    }
    ~GameManager()
    {
        releaseResource();
    }

    // Main game runner
    void runGame();
    void stopGame();

    const Config &getConfig() const;
    int getCurrentTurn() const;
    Player &getCurrentPlayer() const;
    Board &getBoard() const;
    Bank &getBank() const;
    std::vector<Player>& getPlayers() const;
    TransactionLogger &getLogger() const;

    void processMainMenu();
    void processNewGame();
    void processLoadGame();
    void processSaveGame();
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
    void processWin();
};