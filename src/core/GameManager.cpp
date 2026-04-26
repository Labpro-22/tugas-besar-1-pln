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
      skillCardUsedThisTurn{false},
      diceRolledThisTurn{false},
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
            chanceCardDeck += new GoToJailCard(card.message);
        }
        else if (card.type == "GETOUTOFJAIL") {
            chanceCardDeck += new GetOutOfJailCard(card.message);
        }
        else if (card.type == "FORCEDMOVE") {
            chanceCardDeck += new ForcedMoveCard(card.message, card.value);
        }
        else if (card.type == "GOTONEARESTSTATION") {
            chanceCardDeck += new GoToNearestStationCard(card.message);
        }
    }

    // Load community-chest card
    for (CardConfig card : config.communityChestCards) {
        if (card.type == "PAYMONEYCARD") {
            communityChestCardDeck += new PayMoneyCard(card.message, card.value);
        }
        else if (card.type == "PAYMONEYTOPLAYERSCARD") {
            communityChestCardDeck += new PayMoneyToPlayersCard(card.message, card.value);
        }
        else if (card.type == "COLLECTMONEYCARD") {
            communityChestCardDeck += new CollectMoneyCard(card.message, card.value);
        }
        else if (card.type == "COLLECTMONEYFROMPLAYERSCARD") {
            communityChestCardDeck += new CollectMoneyFromPlayersCard(card.message, card.value);
        }
    }

    // Load skill card
    DiceRoller::roll();
    skillCardDeck += new MoveCard("", DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second);
    DiceRoller::roll();
    skillCardDeck += new MoveCard("", DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second);
    DiceRoller::roll();
    skillCardDeck += new MoveCard("", DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second);
    DiceRoller::roll();
    skillCardDeck += new MoveCard("", DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second);
    DiceRoller::roll();
    skillCardDeck += new DiscountCard("", 10 + 3 * (DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second));
    DiceRoller::roll();
    skillCardDeck += new DiscountCard("", 10 + 3 * (DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second));
    DiceRoller::roll();
    skillCardDeck += new DiscountCard("", 10 + 3 * (DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second));
    skillCardDeck += new ShieldCard("");
    skillCardDeck += new ShieldCard("");
    skillCardDeck += new LassoCard("");
    skillCardDeck += new LassoCard("");
    skillCardDeck += new DemolitionCard("");
    skillCardDeck += new DemolitionCard("");
}

GameManager::~GameManager()
{
    for (Card *card : chanceCardDeck.getCards())
        delete card;
    for (Card *card : communityChestCardDeck.getCards())
        delete card;
    for (Card *card : skillCardDeck.getCards())
        delete card;
};

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
    bool maxTurnReached = (config.maxTurn >= 1) && (turn > config.maxTurn);
    return maxTurnReached || activeCount <= 1;
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
            if (startOfTheTurn) {
                processTurnStart();
            }
            gameView.inputNextCommand();
        }
    }
}

void GameManager::processTurnStart()
{
    MainMenuView &view = gameView.getMainMenuView();
    JailView &jailView = gameView.getJailView();
    Player &player = getCurrentPlayer();

    view.outputCurrentPlayerInfo();

    if (player.isJailed()) {
        if (player.getJailTurns() > 3) {
            std::cout << "Sudah 4 giliran di penjara. Kamu wajib membayar denda M" << config.jailFine << ".\n\n";
            try {
                player.payFineToGetOutOfJail(config.jailFine);
                std::cout << "Kamu membayar jaminan sebesar M" << config.jailFine << " dan keluar dari penjara.\n\n";
                logger.log(turn, player.getUsername(), "BAYAR_JAMINAN",
                           "Membayar jaminan paksa M" + std::to_string(config.jailFine) + " (giliran ke-4 di penjara).");
            }
            catch (const PlayerException &e) {
                std::cout << e.what() << std::endl;
            }
        }
        else {
            int choice = jailView.promptRollOrBailOrUseCard();
            if (choice == 1) {
                std::cout << "Gunakan perintah LEMPAR_DADU atau ATUR_DADU untuk mencoba keluar dari penjara.\n\n";
            }
            else if (choice == 2) {
                try {
                    player.payFineToGetOutOfJail(config.jailFine);
                    std::cout << "Kamu membayar jaminan sebesar M" << config.jailFine << " dan keluar dari penjara.\n\n";
                    logger.log(turn, player.getUsername(), "BAYAR_JAMINAN",
                               "Membayar jaminan sebesar M" + std::to_string(config.jailFine) + " untuk keluar dari penjara.");
                }
                catch (const PlayerException &e) {
                    std::cout << e.what() << std::endl;
                }
            }
            else if (choice == 3) {
                try {
                    player.useGetOutOfJailCard();
                    std::cout << "Kamu menggunakan kartu bebas dari penjara.\n\n";
                    logger.log(turn, player.getUsername(), "PAKAI_KARTU_PENJARA",
                               "Menggunakan kartu bebas dari penjara.");
                }
                catch (const PlayerException &e) {
                    std::cout << e.what() << std::endl;
                }
            }
        }
    }
    startOfTheTurn = false;
}

void GameManager::nextTurn()
{
    turn++;

    while (!playerQueue.empty()) {
        playerQueue.pop();
    }

    for (Player &player : players) {
        if (!player.isBankrupt()) {
            playerQueue.push(&player);
        }
    }
}

void GameManager::nextPlayer()
{
    if (!playerQueue.empty()) {
        playerQueue.pop();
    }

    while (!playerQueue.empty() && playerQueue.front()->isBankrupt()) {
        playerQueue.pop();
    }

    if (playerQueue.empty()) {
        nextTurn();
        while (!playerQueue.empty() && playerQueue.front()->isBankrupt()) {
            playerQueue.pop();
        }
    }

    if (playerQueue.empty()) {
        return;
    }
    
    skillCardUsedThisTurn = false;
    diceRolledThisTurn = false;

    Player &player = getCurrentPlayer();
    player.onNextTurn();
    SkillCard* newCard = skillCardDeck.drawCard();
    try {
        player.addSkillCard(newCard);
    }
    catch (const FullHandException &) {
        player.getSkillCardsRef().push_back(newCard);
        std::cout << "\nKamu mendapatkan 1 kartu acak baru!\n";
        std::cout << "Kartu yang didapat: " << newCard->getCardType() << ".\n";
        processDropSkillCard();
    }
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
    else if (option == 2) {
        processLoadGame();
    }
    else {
        running = false;
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
    players.reserve(players.size()); 
    std::shuffle(players.begin(), players.end(), std::default_random_engine{(long unsigned int)time(0)});

    std::vector<Player *> playerPointer;
    for (Player &p : players) {
        playerPointer.push_back(&p);
    }

    // Create board
    board = Board{(int)(config.actionTiles.size() + config.properties.size()), config, playerPointer};

    // Create bank
    bank = Bank{config.initialMoney, config};

    // Clear logger
    logger.clear();

    chanceCardDeck.reshuffle();
    communityChestCardDeck.reshuffle();
    skillCardDeck.reshuffle();

    playing = true;
    initGame();
    nextTurn();
    nextPlayer();
}
void GameManager::processLoadGame()
{
    LoadView &view = gameView.getLoadView();
    view.outputSaveNames();
    std::string saveFileName = view.promptSaveName();
    view.outputLoading();
    try {
        SaveData saveData = SaveFileHandler::loadGame(saveFileName);
        turn = saveData.turn;
        config.maxTurn = saveData.maxTurn;
        players.clear();

        for (PlayerSaveData &playerData : saveData.players) {
            std::vector<SkillCard *> playerCards;
            for (SkillCardSaveData &cardData : playerData.skillCards) {
                if (cardData.type == "DiscountCard") {
                    skillCards.push_back(new DiscountCard("", cardData.value));
                    playerCards.push_back(skillCards.back());
                }
                else if (cardData.type == "LassoCard") {
                    skillCards.push_back(new LassoCard(""));
                    playerCards.push_back(skillCards.back());
                }
                else if (cardData.type == "MoveCard") {
                    skillCards.push_back(new MoveCard("", cardData.value));
                    playerCards.push_back(skillCards.back());
                }
                else if (cardData.type == "TeleportCard") {
                    skillCards.push_back(new TeleportCard(""));
                    playerCards.push_back(skillCards.back());
                }
                if (cardData.type == "DemolitionCard") {
                    skillCards.push_back(new DemolitionCard(""));
                    playerCards.push_back(skillCards.back());
                }
                if (cardData.type == "ShieldCard") {
                    skillCards.push_back(new ShieldCard(""));
                    playerCards.push_back(skillCards.back());
                }
            }
            players.push_back(Player{
                playerData.username,
                playerData.money,
                playerData.status == "ACTIVE"   ? PlayerState::ACTIVE
                : playerData.status == "JAILED" ? PlayerState::JAILED
                                                : PlayerState::BANKRUPT,
                playerCards,
                playerData.getOutOfJailCardCount,
                playerData.jailTurns});
            Player &player = players.back();
            player.getPiece().setPosition(board.getTilePosition(playerData.tileCodePosition));
        }

        std::map<std::string, int> playerOrder;
        int i = 0;
        for (std::string username : saveData.playerOrder) {
            playerOrder[username] = i++;
        }

        std::sort(players.begin(), players.end(), [&](Player &p1, Player &p2) {
            return playerOrder[p1.getUsername()] < playerOrder[p2.getUsername()];
        });

        while (!playerQueue.empty()) {
            playerQueue.pop();
        }

        bool startQueuing = false;
        for (Player& p : players) {
            if (p.getUsername() == saveData.currentPlayer) {
                startQueuing = true;
            }
            if (startQueuing) {
                playerQueue.push(&p);
            }
        }

        bank = Bank{config.initialMoney, config};

        std::vector<Player *> playerPointer;
        for (Player &player : players) {
            playerPointer.push_back(&player);
        }
        board = Board{(int)(config.properties.size() + config.actionTiles.size()), config, playerPointer, saveData.properties};

        logger.clear();
        for (LogSaveData &logData : saveData.logs) {
            logger.log(logData.turn, logData.username, logData.action, logData.details);
        }
        view.outputLoadStatus(true);

        chanceCardDeck.reshuffle();
        communityChestCardDeck.reshuffle();
        skillCardDeck.reshuffle();

        playing = true;
        startOfTheTurn = true;
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
            playerData.getOutOfJailCardCount = player.getGetOutOfJailCardCount();
            playerData.jailTurns = player.getJailTurns();

            for (SkillCard *card : player.getSkillCards()) {
                SkillCardSaveData cardData;
                cardData.type = card->getCardType();
                cardData.value = card->getCardValue();
                // DiscountCard stores remaining duration via PlayerEffect; default 1 for others
                cardData.duration = 1;
                playerData.skillCards.push_back(cardData);
            }
            saveData.players.push_back(playerData);
        }
        saveData.currentPlayer = getCurrentPlayer().getUsername();

        // Build turn order from players vector (sorted by turn order on load)
        // Reconstruct the round-robin order starting from current player
        {
            std::string current = saveData.currentPlayer;
            bool started = false;
            for (Player &p : players) {
                if (p.getUsername() == current) started = true;
                if (started && !p.isBankrupt()) saveData.playerOrder.push_back(p.getUsername());
            }
            for (Player &p : players) {
                if (p.getUsername() == current) break;
                if (!p.isBankrupt()) saveData.playerOrder.push_back(p.getUsername());
            }
        }

        for (Property *property : board.getPropertyList()) {
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
            saveData.deckCards.push_back(card->getCardType());
        }

        for (const TransactionLog &log : logger.getLogs()) {
            LogSaveData logData;
            logData.turn = log.getTurn();
            logData.username = log.getUsername();
            logData.action = log.getAction();
            logData.details = log.getDetails();
            saveData.logs.push_back(logData);
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
    DiceView view = gameView.getDiceView();
    MainMenuView &mainMenuView = gameView.getMainMenuView();
    diceRolledThisTurn = true;
    if (player.getState() == PlayerState::ACTIVE) {
        player.rollDiceAndMove(*this);
        if (player.isJailed()) {
            player.getPiece().setPosition(board.getTilePosition("PEN"));
            view.outputSpeedingToJail(DiceRoller::getLastRoll().first, DiceRoller::getLastRoll().second);
            processGoToJail();
            logger.log(turn, player.getUsername(), "LEMPAR_DADU",
                       "Hasil dadu: " +
                           std::to_string(DiceRoller::getLastRoll().first) + " + " + std::to_string(DiceRoller::getLastRoll().second) + " = " +
                           std::to_string(DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second) +
                           ". Masuk penjara karena mendapat dua mata dadu sama tiga kali.");
            nextPlayer();
        }
        else {
            view.outputRollDice();
            PlayerPiece &piece = player.getPiece();
            piece.getCurrentTile()->onLanded(player, *this);
            mainMenuView.outputCurrentPlayerInfo();
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
        view.outputRollDice(!player.isJailed());
        if (!player.isJailed()) {
            // Escaped jail — move the piece using the roll that freed them
            int total = DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second;
            player.getPiece().goForward(total, player, *this);
            PlayerPiece &piece = player.getPiece();
            piece.getCurrentTile()->onLanded(player, *this);
            mainMenuView.outputCurrentPlayerInfo();
            logger.log(turn, player.getUsername(), "LEMPAR_DADU",
                       "Hasil dadu: " +
                           std::to_string(DiceRoller::getLastRoll().first) + " + " + std::to_string(DiceRoller::getLastRoll().second) + " = " +
                           std::to_string(DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second) +
                           ". Berhasil keluar dari penjara karena dua mata dadu sama. Mendarat di petak " +
                           piece.getCurrentTile()->getName() + " [" + piece.getCurrentTile()->getCode() + "].");
            nextPlayer();
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
    DiceView view = gameView.getDiceView();
    MainMenuView &mainMenuView = gameView.getMainMenuView();
    diceRolledThisTurn = true;
    if (player.getState() == PlayerState::ACTIVE) {
        player.setDiceAndMove(value1, value2, *this); 
        if (player.isJailed()) {
            player.getPiece().setPosition(board.getTilePosition("PEN"));
            view.outputSpeedingToJail(value1, value2);
            processGoToJail();
            logger.log(turn, player.getUsername(), "ATUR_DADU",
                       "Hasil dadu: " +
                           std::to_string(value1) + " + " + std::to_string(value2) + " = " +
                           std::to_string(value1 + value2) +
                           ". Masuk penjara karena mendapat dua mata dadu sama tiga kali.");
            nextPlayer();
        }
        else {
            view.outputSetDice(value1, value2);
            PlayerPiece &piece = player.getPiece();
            piece.getCurrentTile()->onLanded(player, *this);
            mainMenuView.outputCurrentPlayerInfo();
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
        view.outputSetDice(value1, value2, !player.isJailed());
        if (!player.isJailed()) {
            // Escaped jail — move the piece using the set dice values
            player.getPiece().goForward(value1 + value2, player, *this);
            PlayerPiece &piece = player.getPiece();
            piece.getCurrentTile()->onLanded(player, *this);
            mainMenuView.outputCurrentPlayerInfo();
            logger.log(turn, player.getUsername(), "ATUR_DADU",
                       "Hasil dadu: " +
                           std::to_string(value1) + " + " + std::to_string(value2) + " = " +
                           std::to_string(value1 + value2) +
                           ". Berhasil keluar dari penjara karena dua mata dadu sama. Mendarat di petak " +
                           piece.getCurrentTile()->getName() + " [" + piece.getCurrentTile()->getCode() + "].");
            nextPlayer();
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
}
void GameManager::processBuyProperty()
{
    BuyView &view = gameView.getBuyView();

    Player &player = getCurrentPlayer();
    PlayerPiece &piece = player.getPiece();

    PropertyTile *tile = dynamic_cast<PropertyTile *>(piece.getCurrentTile());
    if (tile != nullptr) {
        std::string propType = tile->getProperty()->getPropertyType();
        if (propType != "STREET") {
            // Railroad and Utility: auto-acquire without prompting
            processBuyProperty(player, tile->getProperty());
        }
        else if (player.getMoney() >= tile->getProperty()->getPrice()) {
            if (view.promptBuyProperty(*tile->getProperty())) {
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

    std::vector<Player *> order;
    Player *current = &getCurrentPlayer();
    bool startQueuing = false;
    for (Player &p : players) {
        if (startQueuing && !p.isBankrupt()) order.push_back(&p);
        if (&p == current) startQueuing = true;
    }
    for (Player &p : players) {
        if (&p == current) break;
        if (!p.isBankrupt()) order.push_back(&p);
    }
    if (!current->isBankrupt()) order.push_back(current);

    if (order.empty()) {
        property->resetOwnerAsBank();
        view.outputNoBid(property);
        return;
    }

    std::cout << "Urutan lelang dimulai dari pemain setelah Pemain " << current->getUsername() << ".\n\n";

    Player *lastBidder = nullptr;
    long long bestBidAmount = -1;
    int consecutivePasses = 0;
    int N = (int)order.size();

    int idx = 0;
    while (true) {
        Player *bidder = order[idx % N];
        long long bid = view.promptBidOrPass(*bidder);

        if (bid >= 0 && bid > bestBidAmount) {
            lastBidder = bidder;
            bestBidAmount = bid;
            consecutivePasses = 0;
            std::cout << "Penawaran tertinggi: M" << bestBidAmount << " (Pemain " << lastBidder->getUsername() << ")\n\n";
            logger.log(turn, bidder->getUsername(), "BID",
                       "Bid " + property->getName() + " [" + property->getCode() + "] M" + std::to_string(bid));
        }
        else {
            if (bid >= 0) {
                std::cout << "Bid harus lebih besar dari M" << bestBidAmount << "!\n\n";
                continue;
            }
            consecutivePasses++;
            logger.log(turn, bidder->getUsername(), "PASS",
                       "Pass pelelangan " + property->getName() + " [" + property->getCode() + "]");
        }

        idx++;

        if (lastBidder != nullptr && consecutivePasses >= N - 1) break;
        if (N == 1 && lastBidder != nullptr) break;
        if (lastBidder == nullptr && consecutivePasses >= N) break;
    }

    if (lastBidder == nullptr || bestBidAmount < 0) {
        property->resetOwnerAsBank();
        view.outputNoBid(property);
    }
    else {
        try {
            lastBidder->buyProperty(property, bestBidAmount);
            logger.log(turn, lastBidder->getUsername(), "BELI",
                       "Properti " + property->getName() + " [" + property->getCode() + "] dibeli seharga M" + std::to_string(bestBidAmount));
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

    if (chosenProperty->getPropertyType() == "STREET") {
        StreetProperty* chosenStreet = dynamic_cast<StreetProperty*>(chosenProperty);
        std::string color = chosenStreet->getColor();
        std::vector<StreetProperty*> colorGroupWithBuildings;
        bool hasBuildings = false;
        for (Property* pr : player.getProperties()) {
            if (pr->getColor() == color) {
                if (auto sp = dynamic_cast<StreetProperty*>(pr)) {
                    if (sp->getHouseCount() > 0) {
                        hasBuildings = true;
                        colorGroupWithBuildings.push_back(sp);
                    }
                }
            }
        }
        if (hasBuildings) {
            bool sold = view.sellAllBuildings(colorGroupWithBuildings);
            if (!sold) return;
            for (StreetProperty* sp : colorGroupWithBuildings) {
                long long refund = 0;
                int count = sp->getHouseCount();
                for (int i = 0; i < count; i++) {
                    refund += (sp->hasHotel() ? sp->getHotelPrice() : sp->getHousePrice()) / 2;
                    sp->removeBuilding();
                }
                player.receiveMoney(refund);
            }
        }
        // Ask again to confirm mortgage after buildings sold
        std::string confirm;
        std::cout << "Lanjut menggadaikan " << chosenProperty->getName() << "? (y/n): ";
        std::cin >> confirm;
        if (confirm != "y") return;
    }

    try {
        logger.log(turn, player.getUsername(), "GADAI",
                   chosenProperty->getName() + " [" + chosenProperty->getCode() + "]" +
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

    try {
        player.unmortgageProperty(property);
        gameView.getUnmortgageView().outputUnmortgageStatus(true, *property);
    }
    catch (const PlayerException &e) {
        std::cout << e.what() << std::endl;
        gameView.getUnmortgageView().outputUnmortgageStatus(false, *property);
    }
}
void GameManager::processBuild()
{
    Player &player = getCurrentPlayer();

    BuildView &view = gameView.getBuildView();

    StreetProperty *street = view.promptChooseProperty(player.getProperties());
    if (street != nullptr) {
        try {
            player.buildOnProperty(street);
            if (street->hasHotel()) {
                logger.log(turn, player.getUsername(), "BANGUN",
                           street->getName() + " di-upgrade dan sekarang memiliki hotel");
            } else {
                logger.log(turn, player.getUsername(), "BANGUN",
                           street->getName() + " di-upgrade dan sekarang memiliki " + std::to_string(street->getHouseCount()) + " rumah");
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
    if (diceRolledThisTurn) {
        std::cout << "Kartu kemampuan hanya bisa digunakan SEBELUM melempar dadu.\n\n";
        return;
    }
    if (skillCardUsedThisTurn) {
        std::cout << "Kamu sudah menggunakan kartu kemampuan pada giliran ini!\n";
        std::cout << "Penggunaan kartu dibatasi maksimal 1 kali dalam 1 giliran.\n\n";
        return;
    }

    UseSkillCardView &view = gameView.getUseSkillCardView();
    CardView &cardView = gameView.getCardView();

    int skillIndex = view.promptChooseCardToUse(player.getSkillCards());
    if (skillIndex < 0 || skillIndex >= static_cast<int>(player.getSkillCards().size())) {
        return;
    }

    try {
        SkillCard *card = player.getSkillCards()[skillIndex];
        cardView.outputCard(*card);
        if (card->getCardType() == "DemolitionCard") {
            card->prepareUse(gameView.getUseSkillCardView(), *this);
        }
        else if (card->getCardType() == "LassoCard") {
            card->prepareUse(gameView.getUseSkillCardView(), *this);
        }
        else if (card->getCardType() == "TeleportCard") {
            card->prepareUse(gameView.getUseSkillCardView(), *this);
        }
        player.useSkillCard(skillIndex, *this);
        skillCardUsedThisTurn = true;
        logger.log(turn, player.getUsername(), "USE_CARD",
                   card->getCardType() + " dipakai. " + card->getMessage());
    }
    catch (const PlayerException &e) {
        std::cout << e.what() << std::endl;
    }
}
void GameManager::processDropSkillCard()
{
    Player &player = getCurrentPlayer();

    DropSkillCardView &view = gameView.getDropSkillCardView();

    int skillIndex = view.promptChooseSkillCard(player.getSkillCards());
    if (skillIndex < 0 || skillIndex >= static_cast<int>(player.getSkillCards().size())) {
        return;
    }

    try {
        SkillCard *card = player.getSkillCards()[skillIndex];
        player.dropSkillCard(skillIndex);
        view.outputDropSkillCardStatus(*card);
        logger.log(turn, player.getUsername(), "DROP_CARD",
                   card->getCardType() + " dibuang.");
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

    long long debt = -player.getMoney();
    view.outputPotentialWealth(player, -player.getMoney());

    if (player.calculateTotalWealth() >= -player.getMoney()) {
        while (player.getMoney() < 0) {
            auto chosenProperty = view.promptLiquidation(player.getProperties(), -player.getMoney());
            if (chosenProperty.first == "Batal") {
                if (player.getMoney() < 0) {
                    std::cout << "Kamu masih memiliki kewajiban! Kamu harus melikuidasi aset.\n";
                }
                continue;
            }
            if (chosenProperty.first == "Jual") {
                player.sellProperty(chosenProperty.second);
            }
            else if (chosenProperty.first == "Gadai") {
                player.mortgageProperty(chosenProperty.second);
            }
        }
        // Deduct debt from player (tax goes to bank and is removed from circulation)
        player.deductMoney(debt);
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
void GameManager::processOtherPlayerLiquidation(Player &other)
{
    BankruptView &view = gameView.getBankruptView();

    long long debt = -other.getMoney();
    view.outputPotentialWealth(other, -other.getMoney());

    if (other.calculateTotalWealth() >= -other.getMoney()) {
        while (other.getMoney() < 0) {
            auto chosenProperty = view.promptLiquidation(other.getProperties(), -other.getMoney());
            if (chosenProperty.first == "Jual") {
                other.sellProperty(chosenProperty.second);
            }
            else if (chosenProperty.first == "Gadai") {
                other.mortgageProperty(chosenProperty.second);
            }
        }
        other.deductMoney(debt);
    }
    else {
        view.outputBankruptByBank(other);
        std::vector<Property *> auctionedProperty = other.getProperties();
        other.bankruptByBank();
        for (Property *property : auctionedProperty) {
            processAuctionProperty(property);
        }
    }
}
void GameManager::processOtherPlayerLiquidation(Player &other, Player &creditor)
{
    BankruptView &view = gameView.getBankruptView();

    long long debt = -other.getMoney();
    view.outputPotentialWealth(other, -other.getMoney());

    if (other.calculateTotalWealth() >= -other.getMoney()) {
        while (other.getMoney() < 0) {
            auto chosenProperty = view.promptLiquidation(other.getProperties(), -other.getMoney());
            if (chosenProperty.first == "Jual") {
                other.sellProperty(chosenProperty.second);
            }
            else if (chosenProperty.first == "Gadai") {
                other.mortgageProperty(chosenProperty.second);
            }
        }
        other.deductMoney(debt);
        creditor.receiveMoney(debt);
        view.outputDebtPaid(debt, &creditor);
    }
    else {
        view.outputBankruptByPlayer(other, creditor);
        std::vector<Property *> auctionedProperty = other.getProperties();
        other.bankruptByPlayer(&creditor);
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
            if (chosenProperty.first == "Batal") {
                if (player.getMoney() < 0) {
                    std::cout << "Kamu masih memiliki kewajiban! Kamu harus melikuidasi aset.\n";
                }
                continue;
            }
            if (chosenProperty.first == "Jual") {
                player.sellProperty(chosenProperty.second);
            }
            else if (chosenProperty.first == "Gadai") {
                player.mortgageProperty(chosenProperty.second);
            }
        }
        player.deductMoney(debt);
        creditor.receiveMoney(debt);
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
    std::vector<Player *> remainingPlayer;
    for (Player &player : players) {
        if (!player.isBankrupt()) {
            remainingPlayer.push_back(&player);
        }
    }

    auto cmp = [](const Player *a, const Player *b) {
        if (a->getMoney() != b->getMoney()) return a->getMoney() > b->getMoney();
        if (a->getProperties().size() != b->getProperties().size()) return a->getProperties().size() > b->getProperties().size();
        return a->getSkillCards().size() > b->getSkillCards().size();
    };
    std::sort(remainingPlayer.begin(), remainingPlayer.end(), cmp);

    Player *winner = remainingPlayer.empty() ? nullptr : remainingPlayer.front();

    WinView view = gameView.getWinView();
    view.outputWinner(winner, remainingPlayer);

    playing = false;
    stopGame();
}

void GameManager::processPayRent()
{
    Player &player = getCurrentPlayer();
    PlayerPiece &piece = player.getPiece();

    PropertyTile *tile = dynamic_cast<PropertyTile *>(piece.getCurrentTile());
    Property *prop = tile->getProperty();
    PropertyView &propView = gameView.getPropertyView();
    propView.outputRent(*prop);
    if (prop->getState() == PropertyState::MORTGAGED) {
        return;
    }
    int pay = player.payRent(tile->getProperty());
    if (!pay) {
        processLiquidation(*prop->getOwner());
    }
}

void GameManager::processGoTile()
{
    BoardView &board = gameView.getBoardView();
    board.outputOnPassByStart();
}

void GameManager::processLandingMessage()
{
    // Prints the "Kamu mendarat di X" message for the current player's tile
    gameView.getBoardView().outputOnLanded();
}

void GameManager::processGoToJail()
{
    JailView &jail = gameView.getJailView();
    jail.outputGoToJail();
}

void GameManager::processPayLuxuryTax()
{
    TaxView &taxView = gameView.getTaxView();
    taxView.outputLuxuryTax(config.luxuryFlatTax);

    Player &player = getCurrentPlayer();
    bool payTax = player.payTax(config.luxuryFlatTax);

    if (!payTax) {
        processLiquidation();
    }
}

void GameManager::processPayIncomeTax()
{
    TaxView &taxView = gameView.getTaxView();
    Player &p = getCurrentPlayer();
    int input = taxView.promptIncomeTax(config.incomeFlatTax, config.incomePercentageTax);
    long long amount;
    if (input == 1) {
        amount = config.incomeFlatTax;
    }
    else if (input == 2) {
        amount = p.calculateTotalWealth() * config.incomePercentageTax / 100;
    }
    else {
        return;
    }
    Player &player = getCurrentPlayer();
    bool payTax = player.payTax(amount);
    if (!payTax) {
        processLiquidation();
    }
}

void GameManager::processUseCommunityChestCard()
{
    CardView &cardView = gameView.getCardView();
    CommunityChestCard *card = communityChestCardDeck.drawCard();
    cardView.outputCard(*card);
    Player &p = getCurrentPlayer();
    if ((card->getCardType() == "PAYMONEYCARD" || card->getCardType() == "PAYMONEYTOPLAYERSCARD") && p.hasEffect("SHIELD")) {
        if (p.hasEffect("SHIELD")) {
            cardView.outputShielded();
            return;
        }
    }
    card->takeEffect(p, *this);
}

void GameManager::processUseChanceCard()
{
    CardView &cardView = gameView.getCardView();
    ChanceCard *card = chanceCardDeck.drawCard();
    cardView.outputCard(*card);
    Player &p = getCurrentPlayer();
    if (card->getCardType() == "GOTOJAILCARD") {
        processGoToJail();
    }
    card->takeEffect(p, *this);
}

void GameManager::processStartFestival()
{
    FestivalView &fesView = gameView.getFestivalView();
    Player &p = getCurrentPlayer();
    std::vector<Property *> properties = p.getProperties();
    if (properties.empty()) {
        std::cout << "Kamu tidak memiliki properti. Efek festival tidak dapat digunakan.\n\n";
        return;
    }
    Property *prop = fesView.promptChooseProperty(properties);
    if (prop == nullptr) return;
    prop->startFestival();
    fesView.outputFestivalStatus(*prop);
}
void GameManager::processExit() {
    running = false;
    playing = false;
}