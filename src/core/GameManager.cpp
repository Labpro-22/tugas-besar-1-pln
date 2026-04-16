#include <algorithm>
#include <random>

#include "core/GameManager.hpp"
#include "models/tile/PropertyTile.hpp"

GameManager::GameManager()
    : running{false},
      playing{false},
      config{ConfigLoader::loadConfig()},
      gameView{this},
      turn{0},
      board{config.properties.size(), {}, config.properties, {}},
      players{},
      playerLeaderboard{},
      playerQueue{},
      bank{config.initialMoney},
      logger{},
      chanceCardDeck{},
      communityChestCardDeck{},
      skillCardDeck{} {}

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
bool GameManager::isGameEnded() const
{
    return playerLeaderboard.size() == players.size() - 1 || turn >= config.maxTurn;
}

void GameManager::initGame()
{
    turn = 0;
    bank.giveInitialMoney(getPlayers());
    chanceCardDeck.reshuffle();
    communityChestCardDeck.reshuffle();
    skillCardDeck.reshuffle();
}

void GameManager::gameLoop()
{
    if (!playing) {
        processMainMenu();
    }
    else {
        if (isGameEnded()) {
            processWin();
        }
        else {
            gameView.nextCommand();
            nextPlayer();
        }
    }
}

void GameManager::nextTurn()
{
}

void GameManager::nextPlayer()
{
    playerQueue.pop();
}

// Getters
const Config &GameManager::getConfig() const { return config; }
int GameManager::getCurrentTurn() const { return turn; }
Player &GameManager::getCurrentPlayer() const { return *playerQueue.front(); }
Board &GameManager::getBoard() { return board; }
Bank &GameManager::getBank() { return bank; }
std::vector<Player> &GameManager::getPlayers() { return players; }
TransactionLogger &GameManager::getLogger() { return logger; }

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

    // Create bank
    bank = Bank(config.initialMoney);

    // Clear logger
    logger.clear();

    playing = true;
    initGame();
    nextTurn();
}
void GameManager::processLoadGame()
{
}
void GameManager::processSaveGame()
{
}
void GameManager::processRollDice()
{
    Player &player = getCurrentPlayer();
    player.rollDiceAndMove();
    PlayerPiece &piece = player.getPiece();
    piece.getCurrentTile()->onLanded(player, *this);
}
void GameManager::processSetDice(int value1, int value2)
{
    Player &player = getCurrentPlayer();
    player.setDiceAndMove(value1, value2);
    PlayerPiece &piece = player.getPiece();
    piece.getCurrentTile()->onLanded(player, *this);
}
void GameManager::processBuyProperty()
{
    Player &player = getCurrentPlayer();
    PlayerPiece &piece = player.getPiece();

    PropertyTile *tile = dynamic_cast<PropertyTile *>(piece.getCurrentTile());
    if (tile != nullptr) {
        player.buyProperty(tile->getProperty());
    }
}
void GameManager::processMortgageProperty()
{
    Player &player = getCurrentPlayer();
    MortgageView &view = gameView.getMortgageView();
    int propertyIndex = view.promptChooseProperty(player.getProperties());
    if (propertyIndex == 0) return;

    Property *property = player.getProperties()[propertyIndex];
    player.mortgageProperty(property);
}
void GameManager::processUnmortgageProperty()
{
    Player &player = getCurrentPlayer();
    UnmortgageView &view = gameView.getUnmortgageView();
    int propertyIndex = view.promptChooseProperty(player.getProperties());
    if (propertyIndex == 0) return;

    Property *property = player.getProperties()[propertyIndex];
    StreetProperty *street = dynamic_cast<StreetProperty *>(property);
    if (street != nullptr && (street->getHouseCount() > 0 || street->hasHotel())) {
        street->removeBuilding();
    }
    else {
        player.unmortgageProperty(property);
    }
}
void GameManager::processBuild()
{
    Player &player = getCurrentPlayer();

    BuildView &view = gameView.getBuildView();

    int propertyIndex = view.promptChooseProperty(player.getProperties());
    Property *property = player.getProperties()[propertyIndex];
    StreetProperty *street = dynamic_cast<StreetProperty *>(property);
    if (street != nullptr) {
        if (street->getHouseCount() < 4) {
            street->buildHouse(1);
        }
        else {
            street->buildHotel();
        }
    }
}
void GameManager::processUseSkillCard()
{
    Player &player = getCurrentPlayer();

    UseSkillCardView &view = gameView.getUseSkillCardView();

    int skillIndex = view.promptChooseSkillCard(player.getSkillCards());
    player.useSkillCard(skillIndex);
}
void GameManager::processDropSkillCard() {
    Player &player = getCurrentPlayer();

    DropSkillCardView &view = gameView.getDropSkillCardView();

    int skillIndex = view.promptChooseSkillCard(player.getSkillCards());
    player.dropSkillCard(skillIndex);
}
void GameManager::processLiquidation() {
    Player &player = getCurrentPlayer();

    BankruptView &view = gameView.getBankruptView();
}
void GameManager::processPrintLogs() {
    for (TransactionLog log : logger.getLogs()) {
        std::cout << log.toString() << std::endl;
    }
}