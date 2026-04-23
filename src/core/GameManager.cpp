#include <algorithm>
#include <filesystem>
#include <random>

#include "core/GameManager.hpp"
#include "core/GameManagerException.hpp"
#include "models/card/chance-card/ChanceCard.hpp"
#include "models/card/chance-card/ForcedMoveCard.hpp"
#include "models/card/chance-card/GetOutOfJailCard.hpp"
#include "models/card/chance-card/GoToJailCard.hpp"
#include "models/card/chance-card/GoToNearestStationCard.hpp"
#include "models/card/community-chest-card/CollectMoneyCard.hpp"
#include "models/card/community-chest-card/CollectMoneyFromPlayersCard.hpp"
#include "models/card/community-chest-card/CommunityChestCard.hpp"
#include "models/card/community-chest-card/PayMoneyCard.hpp"
#include "models/card/community-chest-card/PayMoneyToPlayersCard.hpp"
#include "models/card/skill-card/DemolitionCard.hpp"
#include "models/card/skill-card/DiscountCard.hpp"
#include "models/card/skill-card/LassoCard.hpp"
#include "models/card/skill-card/MoveCard.hpp"
#include "models/card/skill-card/ShieldCard.hpp"
#include "models/card/skill-card/SkillCard.hpp"
#include "models/card/skill-card/TeleportCard.hpp"
#include "models/tile/PropertyTile.hpp"

GameManager::GameManager()
    : running{false},
      playing{false},
      startOfTheTurn{false},
      config{ConfigLoader::loadConfig()},
      gameView{*this},
      turn{0},
      board{(int)(config.actionTiles.size() + config.properties.size()), config, {}},
      players{},
      playerQueue{},
      bank{config.initialMoney, config},
      logger{},
      chanceCardDeck{},
      communityChestCardDeck{},
      skillCardDeck{}
{
    // Load chance card
    for (CardConfig card : config.chanceCards) {
        if (card.type == "GOTOJAIL") {
            chanceCardDeck.addCard(new GoToJailCard(card.message));
        }
        else if (card.type == "GETOUTOFJAIL") {
            chanceCardDeck.addCard(new GetOutOfJailCard(card.message));
        }
        else if (card.type == "FORCEDMOVE") {
            chanceCardDeck.addCard(new ForcedMoveCard(card.message, card.value));
        }
        else if (card.type == "GOTONEARESTSTATION") {
            chanceCardDeck.addCard(new GoToNearestStationCard(card.message));
        }
    }

    // Load community-chest card
    for (CardConfig card : config.communityChestCards) {
        if (card.type == "PAY") {
            communityChestCardDeck.addCard(new PayMoneyCard(card.message, card.value));
        }
        else if (card.type == "PAYALL") {
            communityChestCardDeck.addCard(new PayMoneyToPlayersCard(card.message, card.value));
        }
        else if (card.type == "COLLECT") {
            communityChestCardDeck.addCard(new CollectMoneyCard(card.message, card.value));
        }
        else if (card.type == "COLLECTALL") {
            communityChestCardDeck.addCard(new CollectMoneyFromPlayersCard(card.message, card.value));
        }
    }

    // Load skill card
    DiceRoller::roll();
    skillCardDeck.addCard(new MoveCard("", DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second));
    DiceRoller::roll();
    skillCardDeck.addCard(new MoveCard("", DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second));
    DiceRoller::roll();
    skillCardDeck.addCard(new MoveCard("", DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second));
    DiceRoller::roll();
    skillCardDeck.addCard(new MoveCard("", DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second));
    DiceRoller::roll();
    skillCardDeck.addCard(new DiscountCard("", 10 + 3 * (DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second)));
    DiceRoller::roll();
    skillCardDeck.addCard(new DiscountCard("", 10 + 3 * (DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second)));
    DiceRoller::roll();
    skillCardDeck.addCard(new DiscountCard("", 10 + 3 * (DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second)));
    skillCardDeck.addCard(new ShieldCard(""));
    skillCardDeck.addCard(new ShieldCard(""));
    skillCardDeck.addCard(new LassoCard(""));
    skillCardDeck.addCard(new LassoCard(""));
    skillCardDeck.addCard(new DemolitionCard(""));
    skillCardDeck.addCard(new DemolitionCard(""));
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
    if (getCurrentPlayer().getMoney() < 0) {
        processLiquidation();
    }
    while (getCurrentPlayer().isBankrupt()) {
        playerQueue.pop();
    }
    if (playerQueue.empty()) {
        nextTurn();
    }

    Player &player = getCurrentPlayer();
    player.onNextTurn();
    player.addSkillCard(skillCardDeck.drawCard());
    if (player.getMoney() < 0) {
        processLiquidation();
    }
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
CardDeck<ChanceCard *> &GameManager::getChanceCardDeck() { return chanceCardDeck; }
CardDeck<CommunityChestCard *> &GameManager::getCommunityChestCardDeck() { return communityChestCardDeck; }
CardDeck<SkillCard *> &GameManager::getSkillCardDeck() { return skillCardDeck; }

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
    std::shuffle(players.begin(), players.end(), std::default_random_engine{(long unsigned int)time(0)});
    std::vector<Player *> playerPointer;
    for (Player &player : players) {
        playerPointer.push_back(&player);
    }

    // Create board
    board = Board((int)(config.actionTiles.size() + config.properties.size()), config, playerPointer);

    // Create bank
    bank = Bank(config.initialMoney, config);

    // Clear logger
    logger.clear();

    chanceCardDeck.reshuffle();
    communityChestCardDeck.reshuffle();
    skillCardDeck.reshuffle();

    playing = true;
    initGame();
    nextTurn();
}
void GameManager::processLoadGame()
{
    LoadView &view = gameView.getLoadView();
    std::string saveFileName = view.promptSaveName();
    view.outputLoading();
    try {
        SaveData saveData = SaveFileHandler::loadGame(saveFileName);
        view.outputLoadStatus(true);
    }
    catch (const SaveFileNotFoundException &e) {
        std::cout << e.what() << std::endl;
        view.outputLoadStatus(false);
    }
    catch (const SaveFileFormatException &e) {
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

        for (Property *property : bank.getProperties()) {
            PropertySaveData propertyData;
            propertyData.code = property->getCode();
            propertyData.type = property->getPropertyType();
            switch (property->getState()) {
                case PropertyState::BANK:
                    propertyData.owner = "BANK";
                    propertyData.status = "BANK";
                    break;
                case PropertyState::OWNED:
                    propertyData.owner = property->getOwner()->getUsername();
                    propertyData.status = "OWNED";
                    break;
                case PropertyState::MORTGAGED:
                    propertyData.owner = property->getOwner()->getUsername();
                    propertyData.status = "MORTGAGED";
                    break;
                default:
                    propertyData.owner = "UNKNOWN";
                    propertyData.status = "UNKNOWN";
                    break;
            }
            propertyData.festivalMultiplier = property->getFestivalMultiplier();
            propertyData.festivalDuration = property->getFestivalDuration();
            if (property->getPropertyType() == "STREET") {
                StreetProperty *street = dynamic_cast<StreetProperty *>(property);
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
        if (player.getMoney() >= tile->getProperty()->getPrice()) {
            if (tile->getProperty()->getPropertyType() != "STREET" || view.promptBuyProperty(*tile->getProperty())) {
                processBuyProperty(player, tile->getProperty());
            }
            else {
                processAuctionProperty(tile->getProperty());
            }
        }
        else {
            processAuctionProperty(tile->getProperty());
        }
    }
}
void GameManager::processBuyProperty(Player &player, Property *property)
{
    BuyView &view = gameView.getBuyView();

    if (property != nullptr) {
        try {
            bool beli = player.buyProperty(property);
            if (!beli) {
                processAuctionProperty(property);
                return;
            }
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
    Property *property = view.promptChooseProperty(player.getProperties());
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

    view.outputPotentialWealth(player, -player.getMoney());

    if (player.calculateTotalWealth() >= -player.getMoney()) {
        while (player.getMoney() < 0) {
            auto chosenProperty = view.promptLiquidation(player.getProperties(), -player.getMoney());
            if (chosenProperty.first == "Jual") {
                player.sellProperty(chosenProperty.second);
            }
            else if (chosenProperty.first == "Gadai") {
                player.mortgageProperty(chosenProperty.second);
            }
        }
    }
    else {
        view.outputBankruptByBank(player);
        std::vector<Property *> auctionedProperty = player.getProperties();
        player.bankruptByBank();
        for (Property *property : auctionedProperty) {
            processAuctionProperty(property);
        }
    }
}
void GameManager::processLiquidation(Player &creditor)
{
    Player &player = getCurrentPlayer();

    BankruptView &view = gameView.getBankruptView();

    long long debt = -player.getMoney();
    view.outputPotentialWealth(player, -player.getMoney());

    if (player.calculateTotalWealth() >= -player.getMoney()) {
        while (player.getMoney() < 0) {
            auto chosenProperty = view.promptLiquidation(player.getProperties(), -player.getMoney());
            if (chosenProperty.first == "Jual") {
                player.sellProperty(chosenProperty.second);
            }
            else if (chosenProperty.first == "Gadai") {
                player.mortgageProperty(chosenProperty.second);
            }
        }
        view.outputDebtPaid(debt, &creditor);
    }
    else {
        view.outputBankruptByPlayer(player, creditor);
        std::vector<Property *> auctionedProperty = player.getProperties();
        player.bankruptByPlayer(&creditor);
    }
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

void GameManager::processPayRent()
{
    BoardView &boardView = gameView.getBoardView();
    boardView.outputOnLanded();

    Player &player = getCurrentPlayer();
    PlayerPiece &piece = player.getPiece();

    PropertyTile *tile = dynamic_cast<PropertyTile *>(piece.getCurrentTile());
    Property *prop = tile->getProperty();
    PropertyView &propView = gameView.getPropertyView();
    propView.outputRent(*prop);
    int pay = player.payRent(tile->getProperty());
    if (!pay) {
        processLiquidation();
    }
}
