#include <algorithm>
#include <filesystem>
#include <random>

#include "core/GameManager.hpp"
#include "core/GameManagerException.hpp"
#include "models/tile/PropertyTile.hpp"

GameManager::GameManager()
    : running{false},
      playing{false},
      startOfTheTurn{false},
      config{ConfigLoader::loadConfig()},
      gameView{*this},
      turn{0},
      board{(int)config.properties.size(), {}, config, {}},
      players{},
      playerQueue{},
      bank{config.initialMoney, config},
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

bool GameManager::isRunning() const
{
    return running;
}

bool GameManager::isGameEnded() const
{
    int activeCount = 0;
    for (const Player &player : players) {
        if (!player.isBankrupt()) {
            activeCount++;
        }
    }
    return turn > config.maxTurn || activeCount == 1;
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
            gameView.InputNextCommand();
        }
    }
}

void GameManager::nextTurn()
{
    turn++;

    // Reset player queue
    for (Player &player : players) {
        if (!player.isBankrupt()) {
            playerQueue.push(&player);
        }
    }
}

void GameManager::nextPlayer()
{
    while (getCurrentPlayer().isBankrupt()) {
        playerQueue.pop();
    }
    if (playerQueue.empty()) {
        nextTurn();
    }

    Player &player = getCurrentPlayer();
    player.onNextTurn();
    player.addSkillCard(skillCardDeck.drawCard());
    startOfTheTurn = true;
}

// Getters
const Config &GameManager::getConfig() const { return config; }
int GameManager::getCurrentTurn() const { return turn; }
Player &GameManager::getCurrentPlayer() const { return *playerQueue.front(); }
Board &GameManager::getBoard() { return board; }
Bank &GameManager::getBank() { return bank; }
std::vector<Player> &GameManager::getPlayers() { return players; }
TransactionLogger &GameManager::getLogger() { return logger; }
CardDeck<ChanceCard*> &GameManager::getChanceCardDeck() { return chanceCardDeck; }
CardDeck<CommunityChestCard*> &GameManager::getCommunityChestCardDeck() { return communityChestCardDeck; }
CardDeck<SkillCard*> &GameManager::getSkillCardDeck() { return skillCardDeck; }

// Game action
void GameManager::processMainMenu()
{
    MainMenuView mainMenuView = gameView.getMainMenuView();
    int option = mainMenuView.promptNewGameOrLoadGame();
    if (option == 1) {
        processNewGame();
    }
    else {
        processLoadGame();
    }
}
void GameManager::processNewGame()
{
    MainMenuView mainMenuView = gameView.getMainMenuView();

    // Create players
    std::vector<std::string> usernames = mainMenuView.promptUsernames();
    players.clear();
    for (std::string &username : usernames) {
        players.push_back(Player{username, config.initialMoney});
    }
    std::shuffle(players.begin(), players.end(), std::default_random_engine{time(0)});
    std::vector<Player *> playerPointer;
    for (Player &player : players) {
        playerPointer.push_back(&player);
    }

    std::vector<int> tileID = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40};

    // Create board
    board = Board(40, tileID, config, playerPointer);

    // Create bank
    bank = Bank(config.initialMoney, config);

    // Clear logger
    logger.clear();

    playing = true;
    initGame();
    nextTurn();
}
void GameManager::processLoadGame()
{
    LoadView &view = gameView.getLoadView();
    view.outputSaveNames(); // output semua nama save yang ada (tanpa extension)
    std::string saveFileName = view.promptSaveName();
    view.outputLoading();
    try {
        SaveData saveData = SaveFileHandler::loadGame(saveFileName);
        view.outputLoadStatus(true);
    }
    catch (const SaveFileException &e) {
        std::cout << e.what() << std::endl;
        view.outputLoadStatus(false);
    };
}
void GameManager::processSaveGame(std::string fileName)
{
    SaveView &view = gameView.getSaveView();
    if (std::filesystem::exists(fileName) && std::filesystem::is_regular_file(fileName)) {
        if (!view.promptOverwriteSaveFile(fileName)) {
            return;
        }
    }

    view.outputSaving();

    try {
        SaveData saveData;
        saveData.turn = turn;
        saveData.maxTurn = config.maxTurn;
        for (Player &player : players) {
            PlayerSaveData playerData;
            playerData.username = player.getUsername();
            playerData.money = player.getMoney();
            playerData.tileCodePosition = player.getPiece().getCurrentTile()->getCode();
            switch (player.getState()) {
                case PlayerState::ACTIVE:
                    playerData.status = "ACTIVE";
                    break;
                case PlayerState::JAILED:
                    playerData.status = "JAILED";
                    break;
                case PlayerState::BANKRUPT:
                    playerData.status = "BANKRUPT";
                    break;
                default:
                    playerData.status = "UNKNOWN";
                    break;
            }
            for (SkillCard *card : player.getSkillCards()) {
                SkillCardSaveData cardData;
                cardData.type = card->getType();
                cardData.duration = 1;
                cardData.value = 30;
                playerData.skillCards.push_back(cardData);
            }

            playerData.username = player.getUsername();
            saveData.players.push_back(playerData);
            saveData.playerOrder.push_back(player.getUsername());
        }
        saveData.currentPlayer = getCurrentPlayer().getUsername();

        for (Property &property : bank.getProperties()) {
            PropertySaveData propertyData;
            propertyData.code = property.getCode();
            propertyData.type = property.getPropertyType();
            switch (property.getState()) {
                case PropertyState::BANK:
                    propertyData.owner = "BANK";
                    propertyData.status = "BANK";
                    break;
                case PropertyState::OWNED:
                    propertyData.owner = property.getOwner()->getUsername();
                    propertyData.status = "OWNED";
                    break;
                case PropertyState::MORTGAGED:
                    propertyData.owner = property.getOwner()->getUsername();
                    propertyData.status = "MORTGAGED";
                    break;
                default:
                    propertyData.owner = "UNKNOWN";
                    propertyData.status = "UNKNOWN";
                    break;
            }
            propertyData.festivalMultiplier = property.getFestivalMultiplier();
            propertyData.festivalDuration = property.getFestivalDuration();
            if (property.getPropertyType() == "STREET") {
                StreetProperty *street = dynamic_cast<StreetProperty *>(&property);
                propertyData.houseCount = street->getHouseCount();
                propertyData.hasHotel = street->hasHotel();
            }
            else {
                propertyData.houseCount = 0;
                propertyData.hasHotel = false;
            }

            saveData.properties.push_back(propertyData);
        }

        for (SkillCard *card : skillCardDeck.getCards()) {
            saveData.deckCards.push_back(card->getType());
        }

        for (const TransactionLog &log : logger.getLogs()) {
            LogSaveData logData;
            logData.turn = log.getTurn();
            logData.username = log.getUsername();
            logData.action = log.getAction();
            logData.details = log.getDetails();
        }

        SaveFileHandler::saveGame(saveData, fileName);
        logger.log(turn, "Game", "SIMPAN", "Berhasil menyimpan permainan dengan nama " + fileName);
        view.outputSaveStatus(true, fileName);
    }
    catch (const SaveFileException &e) {
        std::cout << e.what() << std::endl;
        logger.log(turn, "Game", "SIMPAN", "Berhasil menyimpan permainan dengan nama " + fileName);
        view.outputSaveStatus(false, fileName);
    }
}
void GameManager::processRollDice()
{
    Player &player = getCurrentPlayer();
    if (player.getState() == PlayerState::ACTIVE) {
        player.rollDiceAndMove();
        if (player.isJailed()) {
            logger.log(turn, player.getUsername(), "LEMPAR_DADU",
                       "Hasil dadu: " +
                           std::to_string(DiceRoller::getLastRoll().first) + " + " + std::to_string(DiceRoller::getLastRoll().second) + " = " +
                           std::to_string(DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second) +
                           ". Masuk penjara karena mendapat dua mata dadu sama tiga kali.");
            nextPlayer();
        }
        else {
            PlayerPiece &piece = player.getPiece();
            piece.getCurrentTile()->onLanded(player, *this);
            if (DiceRoller::getLastRoll().first == DiceRoller::getLastRoll().second) {
                logger.log(turn, player.getUsername(), "LEMPAR_DADU",
                           "Hasil dadu: " +
                               std::to_string(DiceRoller::getLastRoll().first) + " + " + std::to_string(DiceRoller::getLastRoll().second) + " = " +
                               std::to_string(DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second) +
                               ". Mendarat di petak " + piece.getCurrentTile()->getName() + " [ " + piece.getCurrentTile()->getCode() + "]" +
                               ". Mendapatkan giliran tambahan karena dua mata dadu sama");
            }
            else {
                logger.log(turn, player.getUsername(), "LEMPAR_DADU",
                           "Hasil dadu: " +
                               std::to_string(DiceRoller::getLastRoll().first) + " + " + std::to_string(DiceRoller::getLastRoll().second) + " = " +
                               std::to_string(DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second) +
                               ". Mendarat di petak " + piece.getCurrentTile()->getName() + " [ " + piece.getCurrentTile()->getCode() + "]");
                nextPlayer();
            }
        }
    }
    else if (player.getState() == PlayerState::JAILED) {
        player.rollToGetOutOfJail();
        if (!player.isJailed()) {
            logger.log(turn, player.getUsername(), "LEMPAR_DADU",
                       "Hasil dadu: " +
                           std::to_string(DiceRoller::getLastRoll().first) + " + " + std::to_string(DiceRoller::getLastRoll().second) + " = " +
                           std::to_string(DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second) +
                           ". Berhasil keluar dari penjara karena dua mata dadu sama. Mendapat giliran lagi.");
        }
        else {
            logger.log(turn, player.getUsername(), "LEMPAR_DADU",
                       "Hasil dadu: " +
                           std::to_string(DiceRoller::getLastRoll().first) + " + " + std::to_string(DiceRoller::getLastRoll().second) + " = " +
                           std::to_string(DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second) +
                           ". Gagal keluar dari penjara.");
            nextPlayer();
        }
    }
    else if (player.getState() == PlayerState::BANKRUPT) {
        throw AlreadyBankruptException("Player tidak bisa melakukan aksi apapun setelah bangkrut!");
    }
}
void GameManager::processSetDice(int value1, int value2)
{
    Player &player = getCurrentPlayer();
    if (player.getState() == PlayerState::ACTIVE) {
        player.setDiceAndMove(value1, value2);
        if (player.isJailed()) {
            logger.log(turn, player.getUsername(), "ATUR_DADU",
                       "Hasil dadu: " +
                           std::to_string(value1) + " + " + std::to_string(value2) + " = " +
                           std::to_string(value1 + value2) +
                           ". Masuk penjara karena mendapat dua mata dadu sama tiga kali.");
            nextPlayer();
        }
        else {
            PlayerPiece &piece = player.getPiece();
            piece.getCurrentTile()->onLanded(player, *this);
            if (value1 == value2) {
                logger.log(turn, player.getUsername(), "ATUR_DADU",
                           "Hasil dadu: " +
                               std::to_string(value1) + " + " + std::to_string(value2) + " = " +
                               std::to_string(value1 + value2) +
                               ". Mendarat di petak " + piece.getCurrentTile()->getName() + " [ " + piece.getCurrentTile()->getCode() + "]" +
                               ". Mendapatkan giliran tambahan karena dua mata dadu sama");
            }
            else {
                logger.log(turn, player.getUsername(), "ATUR_DADU",
                           "Hasil dadu: " +
                               std::to_string(value1) + " + " + std::to_string(value2) + " = " +
                               std::to_string(value1 + value2) +
                               ". Mendarat di petak " + piece.getCurrentTile()->getName() + " [ " + piece.getCurrentTile()->getCode() + "]");
                nextPlayer();
            }
        }
    }
    else if (player.getState() == PlayerState::JAILED) {
        player.setDiceToGetOutOfJail(value1, value2);
        if (!player.isJailed()) {
            logger.log(turn, player.getUsername(), "ATUR_DADU",
                       "Hasil dadu: " +
                           std::to_string(value1) + " + " + std::to_string(value2) + " = " +
                           std::to_string(value1 + value2) +
                           ". Berhasil keluar dari penjara karena dua mata dadu sama. Mendapat giliran lagi.");
        }
        else {
            logger.log(turn, player.getUsername(), "ATUR_DADU",
                       "Hasil dadu: " +
                           std::to_string(value1) + " + " + std::to_string(value2) + " = " +
                           std::to_string(value1 + value2) +
                           ". Gagal keluar dari penjara.");
            nextPlayer();
        }
    }
    else {
        throw AlreadyBankruptException("Player tidak bisa melakukan aksi apapun setelah bangkrut!");
    }
    nextPlayer();
}
void GameManager::processBuyProperty()
{
    BuyView &view = gameView.getBuyView();

    Player &player = getCurrentPlayer();
    PlayerPiece &piece = player.getPiece();

    PropertyTile *tile = dynamic_cast<PropertyTile *>(piece.getCurrentTile());
    if (tile != nullptr) {
        if (view.promptBuyProperty(*tile->getProperty())) {
            processBuyProperty(player, tile->getProperty());
        }
        else {
            bank.startAuction(*tile->getProperty());
        }
    }
}
void GameManager::processBuyProperty(Player &player, Property *property)
{
    BuyView &view = gameView.getBuyView();

    if (property != nullptr) {
        try {
            player.buyProperty(property);
            logger.log(turn, player.getUsername(), "BELI",
                       "Properti " + property->getName() + " [ " + property->getCode() + "]" +
                           " dibeli seharga " + std::to_string(property->getPrice()));
            view.outputBuyStatus(true, property);
        }
        catch (const PlayerException &e) {
            std::cout << e.what() << std::endl;
            view.outputBuyStatus(false, property);
        }
    }
    else {
        view.outputBuyStatus(false, property);
    }
}
void GameManager::processAuctionProperty(Property *property)
{
    AuctionView &view = gameView.getAuctionView();
    view.outputProperty(*property);

    Player *lastBidder = &getCurrentPlayer();
    long long bestBidAmount = 0;

    auto currentBidder = players.begin();
    while (currentBidder.base() != lastBidder) {
        currentBidder++;
    }
    do {
        long long currentBidAmount = view.promptBidOrPass(*currentBidder);
        if (currentBidAmount != 0) {
            lastBidder = currentBidder.base();
            bestBidAmount = currentBidAmount;
            logger.log(turn, currentBidder->getUsername(), "BID",
                       "Melakukan bid terhadap " + property->getName() + " [ " + property->getCode() + "]" +
                           " sebanyak " + std::to_string(currentBidAmount));
        }
        else {
            logger.log(turn, currentBidder->getUsername(), "PASS",
                       "Melewati pelelangan " + property->getName() + " [ " + property->getCode() + "]");
        }
        currentBidder++;
        if (currentBidder == players.end()) {
            currentBidder = players.begin();
        }
    } while (lastBidder != currentBidder.base());

    if (bestBidAmount == 0) {
        property->resetOwnerAsBank();
        view.outputNoBid(property);
    }
    else {
        try {
            lastBidder->buyProperty(property, bestBidAmount);
            logger.log(turn, lastBidder->getUsername(), "BELI",
                       "Properti " + property->getName() + " [ " + property->getCode() + "]" +
                           " dibeli seharga " + std::to_string(bestBidAmount));
            view.outputWinner(lastBidder, property, bestBidAmount);
        }
        catch (const PlayerException &e) {
            std::cout << e.what() << std::endl;
            view.outputNoBid(property);
        }
    }
}
void GameManager::processMortgageProperty()
{
    Player &player = getCurrentPlayer();
    MortgageView &view = gameView.getMortgageView();
    Property *chosenProperty = view.promptChooseProperty(player.getProperties());
    if (chosenProperty == nullptr) return;
    try {
        logger.log(turn, player.getUsername(), "GADAI",
                   chosenProperty->getName() + " [ " + chosenProperty->getCode() + "]" +
                       " digadaikan seharga " + std::to_string(chosenProperty->getMortgageValue()));
        player.mortgageProperty(chosenProperty);
        view.outputMortgageStatus(true, *chosenProperty);
    }
    catch (const PlayerException &e) {
        std::cout << e.what() << std::endl;
        view.outputMortgageStatus(false, *chosenProperty);
    }
}
void GameManager::processUnmortgageProperty()
{
    Player &player = getCurrentPlayer();
    UnmortgageView &view = gameView.getUnmortgageView();
    Property* property = view.promptChooseProperty(player.getProperties());
    if (property == nullptr) return;

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

    StreetProperty *street = view.promptChooseProperty(player.getProperties());
    if (street != nullptr) {
        try {
            if (street->getHouseCount() < 4) {
                street->buildHouse(1);
                logger.log(turn, player.getUsername(), "BANGUN",
                           street->getName() + " di-upgrade dan sekarang memiliki " + std::to_string(street->getHouseCount()) + " rumah");
            }
            else {
                street->buildHotel();
                logger.log(turn, player.getUsername(), "BANGUN",
                           street->getName() + " di-upgrade dan sekarang memiliki hotel");
            }
            view.outputBuildStatus(true, street);
        }
        catch (StreetPropertyException &e) {
            std::cout << e.what() << std::endl;
            view.outputBuildStatus(false, nullptr);
        }
    }
    else {
        view.outputBuildStatus(false, nullptr);
    }
}
void GameManager::processUseSkillCard()
{
    Player &player = getCurrentPlayer();

    UseSkillCardView &view = gameView.getUseSkillCardView();
    CardView &cardView = gameView.getCardView();

    int skillIndex = view.promptChooseCardToUse(player.getSkillCards());
    try {
        SkillCard *card = player.getSkillCards()[skillIndex - 1];
        cardView.outputCard(*card);
        player.useSkillCard(skillIndex, *this);
        logger.log(turn, player.getUsername(), "USE_CARD",
                   card->getType() + " dipakai. " + card->getMessage());
    }
    catch (const PlayerException &e) {
        std::cout << e.what() << std::endl;
    }
    catch (...) {
    }

    // TODO: efek efek skill card
}
void GameManager::processDropSkillCard()
{
    Player &player = getCurrentPlayer();

    DropSkillCardView &view = gameView.getDropSkillCardView();

    int skillIndex = view.promptChooseSkillCard(player.getSkillCards());
    try {
        SkillCard *card = player.getSkillCards()[skillIndex - 1];
        player.dropSkillCard(skillIndex - 1);
        view.outputDropSkillCardStatus(*card);
        logger.log(turn, player.getUsername(), "DROP_CARD",
                   card->getType() + " dibuang.");
    }
    catch (const PlayerException &e) {
        std::cout << e.what() << std::endl;
    }
    catch (...) {
    }
}
void GameManager::processLiquidation()
{
    Player &player = getCurrentPlayer();

    BankruptView &view = gameView.getBankruptView();
}

void GameManager::processWin()
{
    Player *winner = nullptr;
    std::vector<Player *> remainingPlayer;
    for (Player &player : players) {
        if (!player.isBankrupt()) {
            remainingPlayer.push_back(&player);
            if (winner == nullptr || winner->calculateTotalWealth() < player.calculateTotalWealth()) {
                winner = &player;
            }
        }
    }

    WinView view = gameView.getWinView();
    view.outputWinner(winner, remainingPlayer);

    playing = false;
}