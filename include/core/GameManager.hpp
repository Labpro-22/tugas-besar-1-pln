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
#include "models/player/PlayerPiece.hpp"
#include "models/property/Property.hpp"
#include "models/tile/Tile.hpp"
#include "utils/config/Config.hpp"
#include "utils/config/ConfigLoader.hpp"
#include "utils/log/TransactionLogger.hpp"
#include "utils/save-load/SaveData.hpp"
#include "utils/save-load/SaveFileHandler.hpp"
#include "views/GameView.hpp"

class GameManager {
private:
    bool running;
    bool playing;
    bool startOfTheTurn;
    Config config;
    GameView gameView;
    int turn;
    Board board;
    std::vector<Player> players;
    std::queue<Player *> playerQueue;
    Bank bank;
    TransactionLogger logger;
    CardDeck<ChanceCard> chanceCardDeck;
    CardDeck<CommunityChestCard> communityChestCardDeck;
    CardDeck<SkillCard> skillCardDeck;

    // main game runner
    void gameLoop();
    bool isRunning() const;
    bool isGameEnded() const;
    void initGame();
    void nextTurn();
    void nextPlayer();

public:
    GameManager();

    // Main game runner
    void runGame();
    void stopGame();

    const Config &getConfig() const;
    int getCurrentTurn() const;
    Player &getCurrentPlayer() const;
    Board &getBoard();
    Bank &getBank();
    std::vector<Player> &getPlayers();
    TransactionLogger &getLogger();
    CardDeck<ChanceCard> &getChanceCardDeck();
    CardDeck<CommunityChestCard> &getCommunityChestCardDeck();
    CardDeck<SkillCard> &getSkillCardDeck();

    void processMainMenu();
    void processNewGame();
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