#include <algorithm>
#include <filesystem>
#include <random>
#include <sstream>

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

namespace {
    SkillCard *createSkillCard(const std::string &type, int value) {
        if (type == "DiscountCard") {
            return new DiscountCard("", value);
        }
        if (type == "LassoCard") {
            return new LassoCard("");
        }
        if (type == "MoveCard") {
            return new MoveCard("", value);
        }
        if (type == "TeleportCard") {
            return new TeleportCard("");
        }
        if (type == "DemolitionCard") {
            return new DemolitionCard("");
        }
        if (type == "ShieldCard") {
            return new ShieldCard("");
        }
        return nullptr;
    }

    SkillCardSaveData serializeSkillCard(const SkillCard *card) {
        SkillCardSaveData data;
        data.type = card->getCardType();
        data.duration = 1;
        data.value = 0;

        if (const auto *moveCard = dynamic_cast<const MoveCard *>(card)) {
            data.value = moveCard->getAmount();
        } else if (const auto *discountCard = dynamic_cast<const DiscountCard *>(card)) {
            data.value = discountCard->getPercentage();
        }

        return data;
    }

    std::string serializeSkillCardLine(const SkillCard *card) {
        SkillCardSaveData data = serializeSkillCard(card);
        std::ostringstream stream;
        stream << data.type << " " << data.value << " " << data.duration;
        return stream.str();
    }

    SkillCardSaveData parseSkillCardLine(const std::string &line) {
        SkillCardSaveData data;
        data.type = "";
        data.value = 0;
        data.duration = 1;

        std::stringstream stream(line);
        stream >> data.type;
        stream >> data.value;
        stream >> data.duration;

        return data;
    }

    void clearOwnedSkillCards(std::vector<SkillCard *> &cards) {
        for (SkillCard *card : cards) {
            delete card;
        }
        cards.clear();
    }

    void resetSkillDeckToDefault(CardDeck<SkillCard *> &deck) {
        deck.reshuffle();
        for (SkillCard *card : deck.getCards()) {
            delete card;
        }
        deck.clearDeck();

        DiceRoller::roll();
        deck += new MoveCard("", DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second);
        DiceRoller::roll();
        deck += new MoveCard("", DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second);
        DiceRoller::roll();
        deck += new MoveCard("", DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second);
        DiceRoller::roll();
        deck += new MoveCard("", DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second);
        DiceRoller::roll();
        deck += new DiscountCard("", 10 + 3 * (DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second));
        DiceRoller::roll();
        deck += new DiscountCard("", 10 + 3 * (DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second));
        DiceRoller::roll();
        deck += new DiscountCard("", 10 + 3 * (DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second));
        deck += new ShieldCard("");
        deck += new ShieldCard("");
        deck += new LassoCard("");
        deck += new LassoCard("");
        deck += new DemolitionCard("");
        deck += new DemolitionCard("");
    }
}

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
      skillCardDeck{} {
    // Load chance card
    for (CardConfig card : config.chanceCards) {
        if (card.type == "GOTOJAIL") {
            chanceCardDeck += new GoToJailCard(card.message);
        } else if (card.type == "GETOUTOFJAIL") {
            chanceCardDeck += new GetOutOfJailCard(card.message);
        } else if (card.type == "FORCEDMOVE") {
            chanceCardDeck += new ForcedMoveCard(card.message, card.value);
        } else if (card.type == "GOTONEARESTSTATION") {
            chanceCardDeck += new GoToNearestStationCard(card.message);
        }
    }

    // Load community-chest card
    for (CardConfig card : config.communityChestCards) {
        if (card.type == "PAYMONEYCARD") {
            communityChestCardDeck += new PayMoneyCard(card.message, card.value);
        } else if (card.type == "PAYMONEYTOPLAYERSCARD") {
            communityChestCardDeck += new PayMoneyToPlayersCard(card.message, card.value);
        } else if (card.type == "COLLECTMONEYCARD") {
            communityChestCardDeck += new CollectMoneyCard(card.message, card.value);
        } else if (card.type == "COLLECTMONEYFROMPLAYERSCARD") {
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

GameManager::~GameManager() {
    chanceCardDeck.reshuffle();
    communityChestCardDeck.reshuffle();
    skillCardDeck.reshuffle();
    for (Card *card : chanceCardDeck.getCards())
        delete card;
    for (Card *card : communityChestCardDeck.getCards())
        delete card;
    for (Card *card : skillCardDeck.getCards())
        delete card;
    clearOwnedSkillCards(skillCards);
};

// Game runner
void GameManager::runGame() {
    running = true;
    while (running) {
        gameLoop();
    }
}

void GameManager::stopGame() {
    running = false;
}

bool GameManager::isRunning() const {
    return running;
}

bool GameManager::isGameEnded() const {
    int activeCount = 0;
    for (const Player &player : players) {
        if (!player.isBankrupt()) {
            activeCount++;
        }
    }
    bool maxTurnReached = (config.maxTurn >= 1) && (turn > config.maxTurn);
    return maxTurnReached || activeCount <= 1;
}

void GameManager::initGame() {
    turn = 0;
    bank.giveInitialMoney(getPlayers());
    chanceCardDeck.reshuffle();
    communityChestCardDeck.reshuffle();
    skillCardDeck.reshuffle();
}

void GameManager::gameLoop() {
    if (!playing) {
        processMainMenu();
    } else {
        if (getCurrentPlayer().isBankrupt()) {
            nextPlayer();
        }
        if (isGameEnded()) {
            processWin();
        } else {
            if (startOfTheTurn) {
                processTurnStart();
            }
            gameView.inputNextCommand();
        }
    }
}

void GameManager::processTurnStart() {
    MainMenuView &view = gameView.getMainMenuView();
    JailView &jailView = gameView.getJailView();
    Player &player = getCurrentPlayer();

    view.outputCurrentPlayerInfo();

    if (player.isJailed()) {
        if (player.getJailTurns() > 3) {
            std::cout << "Sudah 4 giliran di penjara. Kamu wajib membayar denda M" << config.jailFine << ".\n\n";
            try {
                std::cout << "Kamu membayar jaminan sebesar M" << config.jailFine << " dan keluar dari penjara.\n\n";
                logger.log(turn, player.getUsername(), "BAYAR_JAMINAN",
                           "Membayar jaminan paksa M" + std::to_string(config.jailFine) + " (giliran ke-4 di penjara).");
                if (!player.payFineToGetOutOfJail(config.jailFine)) {
                    processLiquidation();
                    nextPlayer();
                    return;
                }
            } catch (const PlayerException &e) {
                std::cout << e.what() << std::endl;
            }
        } else {
            int choice = jailView.promptRollOrBailOrUseCard();
            if (choice == 1) {
                std::cout << "Gunakan perintah LEMPAR_DADU atau ATUR_DADU untuk mencoba keluar dari penjara.\n\n";
            } else if (choice == 2) {
                try {
                    std::cout << "Kamu membayar jaminan sebesar M" << config.jailFine << " dan keluar dari penjara.\n\n";
                    logger.log(turn, player.getUsername(), "BAYAR_JAMINAN",
                               "Membayar jaminan sebesar M" + std::to_string(config.jailFine) + " untuk keluar dari penjara.");
                    if (!player.payFineToGetOutOfJail(config.jailFine)) {
                        processLiquidation();
                        nextPlayer();
                        return;
                    }
                } catch (const PlayerException &e) {
                    std::cout << e.what() << std::endl;
                }
            } else if (choice == 3) {
                try {
                    player.useGetOutOfJailCard();
                    std::cout << "Kamu menggunakan kartu bebas dari penjara.\n\n";
                    logger.log(turn, player.getUsername(), "PAKAI_KARTU_PENJARA",
                               "Menggunakan kartu bebas dari penjara.");
                } catch (const PlayerException &e) {
                    std::cout << e.what() << std::endl;
                }
            }
        }
    }
    startOfTheTurn = false;
}

void GameManager::nextTurn() {
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

void GameManager::startCurrentPlayerTurn() {
    if (playerQueue.empty()) {
        return;
    }

    skillCardUsedThisTurn = false;
    diceRolledThisTurn = false;

    Player &player = getCurrentPlayer();
    player.onNextTurn();

    // FIX: draw board at start of turn so player can see board state
    gameView.getBoardView().drawBoard();

    SkillCard *drawnCard = skillCardDeck.drawCard();
    bool shouldKeepDrawnCard = true;
    bool cardAlreadyShown = false;
    if (player.getSkillCards().size() >= 3) {
        UseSkillCardView &skillView = gameView.getUseSkillCardView();
        DropSkillCardView &dropView = gameView.getDropSkillCardView();
        skillView.outputReceivedCard(*drawnCard);
        cardAlreadyShown = true;

        int droppedIndex = dropView.promptChooseSkillCard(player.getSkillCards());
        if (droppedIndex < 0) {
            skillCardDeck.returnDrawnCard(drawnCard);
            shouldKeepDrawnCard = false;
            std::cout << "Kartu baru ditolak dan dikembalikan ke deck.\n\n";
            logger.log(turn, player.getUsername(), "REJECT_CARD",
                       drawnCard->getCardType() + " ditolak karena tangan penuh.");
        } else {
            SkillCard *droppedCard = player.getSkillCards()[droppedIndex];
            player.dropSkillCard(droppedIndex);
            dropView.outputDropSkillCardStatus(*droppedCard);
            logger.log(turn, player.getUsername(), "DROP_CARD",
                       droppedCard->getCardType() + " dibuang untuk memberi ruang kartu baru.");
        }
    }

    if (shouldKeepDrawnCard) {
        player.addSkillCard(drawnCard);
        if (!cardAlreadyShown) {
            gameView.getUseSkillCardView().outputReceivedCard(*drawnCard);
        }
    }
    if (player.getMoney() < 0) {
        processLiquidation();
    }
    startOfTheTurn = true;
}

void GameManager::nextPlayer() {
    if (!playerQueue.empty()) {
        playerQueue.front()->removeEffect("SHIELD");
        playerQueue.front()->resetDoubleRollCounter();
    }

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

    if (playerQueue.empty() || isGameEnded()) {
        return;
    }

    startCurrentPlayerTurn();
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
void GameManager::processMainMenu() {
    MainMenuView mainMenuView = gameView.getMainMenuView();
    int option = mainMenuView.promptNewGameOrLoadGame();
    if (option == 1) {
        processNewGame();
    } else if (option == 2) {
        processLoadGame();
    } else {
        running = false;
    }
}
void GameManager::processNewGame() {
    MainMenuView mainMenuView = gameView.getMainMenuView();

    clearOwnedSkillCards(skillCards);
    resetSkillDeckToDefault(skillCardDeck);

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
    startCurrentPlayerTurn();
}
void GameManager::processLoadGame() {
    LoadView &view = gameView.getLoadView();
    view.outputSaveNames();
    std::string saveFileName = view.promptSaveName();
    view.outputLoading();
    try {
        SaveData saveData = SaveFileHandler::loadGame(saveFileName);
        turn = saveData.turn;
        config.maxTurn = saveData.maxTurn;
        clearOwnedSkillCards(skillCards);
        players.clear();

        for (PlayerSaveData &playerData : saveData.players) {
            std::vector<SkillCard *> playerCards;
            for (SkillCardSaveData &cardData : playerData.skillCards) {
                SkillCard *card = createSkillCard(cardData.type, cardData.value);
                if (card != nullptr) {
                    skillCards.push_back(card);
                    playerCards.push_back(card);
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
                // FIX: include doubleRollCounter for proper jail state restoration
                playerData.jailTurns,
                playerData.doubleRollCounter});
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
        for (Player &p : players) {
            if (p.getUsername() == saveData.currentPlayer) {
                startQueuing = true;
            }
            if (startQueuing) {
                playerQueue.push(&p);
            }
        }
        for (Player &p : players) {
            if (p.getUsername() == saveData.currentPlayer) {
                break;
            }
            playerQueue.push(&p);
        }

        bank = Bank{config.initialMoney, config};

        std::vector<Player *> playerPointer;
        for (Player &player : players) {
            playerPointer.push_back(&player);
        }
        board = Board{(int)(config.properties.size() + config.actionTiles.size()), config, playerPointer, saveData.properties};

        for (PlayerSaveData &playerData : saveData.players) {
            auto playerIt = std::find_if(players.begin(), players.end(), [&](Player &player) {
                return player.getUsername() == playerData.username;
            });
            if (playerIt != players.end()) {
                playerIt->getPiece().setPosition(board.getTilePosition(playerData.tileCodePosition));
            }
        }

        logger.clear();
        for (LogSaveData &logData : saveData.logs) {
            logger.log(logData.turn, logData.username, logData.action, logData.details);
        }
        view.outputLoadStatus(true);

        chanceCardDeck.reshuffle();
        communityChestCardDeck.reshuffle();
        if (saveData.deckCards.empty()) {
            resetSkillDeckToDefault(skillCardDeck);
        } else {
            skillCardDeck.reshuffle();
            for (SkillCard *card : skillCardDeck.getCards()) {
                delete card;
            }
            skillCardDeck.clearDeck();
            for (const std::string &cardLine : saveData.deckCards) {
                SkillCardSaveData cardData = parseSkillCardLine(cardLine);
                SkillCard *card = createSkillCard(cardData.type, cardData.value);
                if (card != nullptr) {
                    skillCardDeck += card;
                }
            }
        }

        playing = true;
        startOfTheTurn = true;
        logger.log(turn, "GAME", "LOAD",
                   "Berhasil memuat " + saveFileName);
    } catch (const SaveFileNotFoundException &e) {
        std::cout << e.what() << std::endl;
        view.outputLoadStatus(false);
    } catch (const SaveFileFormatException &e) {
        std::cout << e.what() << std::endl;
        view.outputLoadStatus(false);
    };
}
void GameManager::processSaveGame(std::string fileName) {
    SaveView &view = gameView.getSaveView();
    std::filesystem::path savePath = std::filesystem::path("data") / fileName;
    if (std::filesystem::exists(savePath) && std::filesystem::is_regular_file(savePath)) {
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
            playerData.doubleRollCounter = player.getDoubleRollCounter();

            for (SkillCard *card : player.getSkillCards()) {
                // FIX: use serializeSkillCard for proper type-aware serialization
                playerData.skillCards.push_back(serializeSkillCard(card));
            }
            saveData.players.push_back(playerData);
        }
        saveData.currentPlayer = getCurrentPlayer().getUsername();

        std::string current = saveData.currentPlayer;
        for (Player &p : players) {
            saveData.playerOrder.push_back(p.getUsername());
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
            } else {
                propertyData.houseCount = 0;
                propertyData.hasHotel = false;
            }

            saveData.properties.push_back(propertyData);
        }

        for (SkillCard *card : skillCardDeck.getCards()) {
            saveData.deckCards.push_back(serializeSkillCardLine(card));
        }

        logger.log(turn, "GAME", "SIMPAN", "Berhasil menyimpan permainan dengan nama " + fileName);
        for (const TransactionLog &log : logger.getLogs()) {
            LogSaveData logData;
            logData.turn = log.getTurn();
            logData.username = log.getUsername();
            logData.action = log.getAction();
            logData.details = log.getDetails();
            saveData.logs.push_back(logData);
        }

        SaveFileHandler::saveGame(saveData, fileName);
        view.outputSaveStatus(true, fileName);
    } catch (const SaveFileException &e) {
        std::cout << e.what() << std::endl;
        logger.log(turn, "GAME", "SIMPAN", "Gagal menyimpan permainan dengan nama " + fileName);
        view.outputSaveStatus(false, fileName);
    }
}
void GameManager::processRollDice() {
    Player &player = getCurrentPlayer();
    DiceView view = gameView.getDiceView();
    MainMenuView &mainMenuView = gameView.getMainMenuView();
    diceRolledThisTurn = true;
    if (player.getState() == PlayerState::ACTIVE) {
        bool passedStart = player.rollDiceAndMove();
        if (player.isJailed()) {
            player.getPiece().setPosition(board.getTilePosition("PEN"));
            view.outputSpeedingToJail(DiceRoller::getLastRoll().first, DiceRoller::getLastRoll().second);
            processGoToJail("Double tiga kali berturut-turut.");
            logger.log(turn, player.getUsername(), "LEMPAR_DADU",
                       "Hasil dadu: " +
                           std::to_string(DiceRoller::getLastRoll().first) + " + " + std::to_string(DiceRoller::getLastRoll().second) + " = " +
                           std::to_string(DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second) +
                           ". Masuk penjara karena mendapat dua mata dadu sama tiga kali.");
            nextPlayer();
        } else {
            view.outputRollDice();
            if (passedStart) {
                board.getTile(0)->onPassBy(player, *this);
            }
            PlayerPiece &piece = player.getPiece();
            view.outputLandedOnTile(*piece.getCurrentTile());
            piece.getCurrentTile()->onLanded(player, *this);
            if (DiceRoller::getLastRoll().first == DiceRoller::getLastRoll().second) {
                gameView.getBoardView().drawBoard();
                mainMenuView.outputCurrentPlayerInfo();
                logger.log(turn, player.getUsername(), "LEMPAR_DADU",
                           "Hasil dadu: " +
                               std::to_string(DiceRoller::getLastRoll().first) + " + " + std::to_string(DiceRoller::getLastRoll().second) + " = " +
                               std::to_string(DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second) +
                               ". Mendarat di petak " + piece.getCurrentTile()->getName() + " [ " + piece.getCurrentTile()->getCode() + "]" +
                               ". Mendapatkan giliran tambahan karena dua mata dadu sama");
            } else {
                logger.log(turn, player.getUsername(), "LEMPAR_DADU",
                           "Hasil dadu: " +
                               std::to_string(DiceRoller::getLastRoll().first) + " + " + std::to_string(DiceRoller::getLastRoll().second) + " = " +
                               std::to_string(DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second) +
                               ". Mendarat di petak " + piece.getCurrentTile()->getName() + " [ " + piece.getCurrentTile()->getCode() + "]");
                nextPlayer();
            }
        }
    } else if (player.getState() == PlayerState::JAILED) {
        player.rollToGetOutOfJail();
        view.outputJailRollDice(!player.isJailed());
        if (!player.isJailed()) {
            diceRolledThisTurn = false;
            std::cout << "Kamu berhasil keluar dari penjara. Lempar dadu lagi untuk bergerak.\n\n";
            gameView.getBoardView().drawBoard();
            mainMenuView.outputCurrentPlayerInfo();
            logger.log(turn, player.getUsername(), "LEMPAR_DADU",
                       "Hasil dadu: " +
                           std::to_string(DiceRoller::getLastRoll().first) + " + " + std::to_string(DiceRoller::getLastRoll().second) + " = " +
                           std::to_string(DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second) +
                           ". Berhasil keluar dari penjara karena dua mata dadu sama. Harus melempar dadu lagi untuk bergerak.");
        } else {
            logger.log(turn, player.getUsername(), "LEMPAR_DADU",
                       "Hasil dadu: " +
                           std::to_string(DiceRoller::getLastRoll().first) + " + " + std::to_string(DiceRoller::getLastRoll().second) + " = " +
                           std::to_string(DiceRoller::getLastRoll().first + DiceRoller::getLastRoll().second) +
                           ". Gagal keluar dari penjara.");
            nextPlayer();
        }
    } else if (player.getState() == PlayerState::BANKRUPT) {
        throw AlreadyBankruptException("Player tidak bisa melakukan aksi apapun setelah bangkrut!");
    }
}
void GameManager::processSetDice(int value1, int value2) {
    // if (value1 < 1 || value1 > 6 || value2 < 1 || value2 > 6) {
    //     std::cout << "Nilai dadu harus antara 1 sampai 6.\n\n";
    //     return;
    // }

    Player &player = getCurrentPlayer();
    DiceView view = gameView.getDiceView();
    MainMenuView &mainMenuView = gameView.getMainMenuView();
    diceRolledThisTurn = true;
    if (player.getState() == PlayerState::ACTIVE) {
        bool passedStart = player.setDiceAndMove(value1, value2);
        if (player.isJailed()) {
            player.getPiece().setPosition(board.getTilePosition("PEN"));
            view.outputSpeedingToJail(value1, value2);
            processGoToJail("Double tiga kali berturut-turut.");
            logger.log(turn, player.getUsername(), "ATUR_DADU",
                       "Hasil dadu: " +
                           std::to_string(value1) + " + " + std::to_string(value2) + " = " +
                           std::to_string(value1 + value2) +
                           ". Masuk penjara karena mendapat dua mata dadu sama tiga kali.");
            nextPlayer();
        } else {
            view.outputSetDice(value1, value2);
            if (passedStart) {
                board.getTile(0)->onPassBy(player, *this);
            }
            PlayerPiece &piece = player.getPiece();
            view.outputLandedOnTile(*piece.getCurrentTile());
            piece.getCurrentTile()->onLanded(player, *this);
            if (value1 == value2) {
                gameView.getBoardView().drawBoard();
                mainMenuView.outputCurrentPlayerInfo();
                logger.log(turn, player.getUsername(), "ATUR_DADU",
                           "Hasil dadu: " +
                               std::to_string(value1) + " + " + std::to_string(value2) + " = " +
                               std::to_string(value1 + value2) +
                               ". Mendarat di petak " + piece.getCurrentTile()->getName() + " [ " + piece.getCurrentTile()->getCode() + "]" +
                               ". Mendapatkan giliran tambahan karena dua mata dadu sama");
            } else {
                logger.log(turn, player.getUsername(), "ATUR_DADU",
                           "Hasil dadu: " +
                               std::to_string(value1) + " + " + std::to_string(value2) + " = " +
                               std::to_string(value1 + value2) +
                               ". Mendarat di petak " + piece.getCurrentTile()->getName() + " [ " + piece.getCurrentTile()->getCode() + "]");
                nextPlayer();
            }
        }
    } else if (player.getState() == PlayerState::JAILED) {
        player.setDiceToGetOutOfJail(value1, value2);
        view.outputJailSetDice(value1, value2, !player.isJailed());
        if (!player.isJailed()) {
            diceRolledThisTurn = false;
            std::cout << "Kamu berhasil keluar dari penjara. Lempar dadu lagi untuk bergerak.\n\n";
            gameView.getBoardView().drawBoard();
            mainMenuView.outputCurrentPlayerInfo();
            logger.log(turn, player.getUsername(), "ATUR_DADU",
                       "Hasil dadu: " +
                           std::to_string(value1) + " + " + std::to_string(value2) + " = " +
                           std::to_string(value1 + value2) +
                           ". Berhasil keluar dari penjara karena dua mata dadu sama. Harus melempar dadu lagi untuk bergerak.");
        } else {
            logger.log(turn, player.getUsername(), "ATUR_DADU",
                       "Hasil dadu: " +
                           std::to_string(value1) + " + " + std::to_string(value2) + " = " +
                           std::to_string(value1 + value2) +
                           ". Gagal keluar dari penjara.");
            nextPlayer();
        }
    } else {
        throw AlreadyBankruptException("Player tidak bisa melakukan aksi apapun setelah bangkrut!");
    }
}
void GameManager::processBuyProperty() {
    Player &player = getCurrentPlayer();
    processBuyProperty(player);
}

void GameManager::processBuyProperty(Player &player) {
    BuyView &view = gameView.getBuyView();
    PlayerPiece &piece = player.getPiece();

    PropertyTile *tile = dynamic_cast<PropertyTile *>(piece.getCurrentTile());
    if (tile != nullptr) {
        std::string propType = tile->getProperty()->getPropertyType();
        // FIX: Railroad and Utility auto-acquire without prompt or auction; only Street goes to auction
        if (propType != "STREET") {
            processBuyProperty(player, tile->getProperty());
        } else if (player.getMoney() >= tile->getProperty()->getPrice()) {
            if (view.promptBuyProperty(*tile->getProperty(), player)) {
                processBuyProperty(player, tile->getProperty());
            } else {
                processAuctionProperty(tile->getProperty(), &player);
            }
        } else {
            processAuctionProperty(tile->getProperty(), &player);
        }
    }
}
void GameManager::processBuyProperty(Player &player, Property *property) {
    BuyView &view = gameView.getBuyView();

    if (property != nullptr) {
        try {
            long long buyPrice = property->getPrice();
            if (player.hasEffect("DISCOUNT")) {
                buyPrice = buyPrice * (100 - player.getEffectValue("DISCOUNT")) / 100;
            }
            bool beli = player.buyProperty(property);
            if (!beli) {
                processAuctionProperty(property);
                return;
            }
            logger.log(turn, player.getUsername(), "BELI",
                       "Properti " + property->getName() + " [ " + property->getCode() + "]" +
                           " dibeli seharga " + std::to_string(buyPrice));
            view.outputBuyStatus(true, property, player);
        } catch (const PlayerException &e) {
            std::cout << e.what() << std::endl;
            view.outputBuyStatus(false, property, player);
        }
    } else {
        view.outputBuyStatus(false, property, player);
    }
}
void GameManager::processAuctionProperty(Property *property, Player *excludedPlayer) {
    AuctionView &view = gameView.getAuctionView();
    if (property == nullptr) {
        return;
    }

    view.outputProperty(*property);
    if (excludedPlayer != nullptr) {
        view.outputExcludedPlayer(*excludedPlayer);
    }

    std::vector<Player *> bidders;
    size_t currentBidderIndex = 0;
    if (excludedPlayer != nullptr) {
        auto playerIt = std::find_if(players.begin(), players.end(), [&](const Player &player) {
            return &player == excludedPlayer;
        });
        if (playerIt != players.end()) {
            size_t rejectedPlayerIndex = static_cast<size_t>(std::distance(players.begin(), playerIt));
            for (size_t offset = 1; offset < players.size(); ++offset) {
                Player *candidate = &players[(rejectedPlayerIndex + offset) % players.size()];
                if (!candidate->isBankrupt()) {
                    bidders.push_back(candidate);
                }
            }
        }
        if (!excludedPlayer->isBankrupt()) {
            bidders.push_back(excludedPlayer);
        }
    } else {
        for (Player &player : players) {
            if (!player.isBankrupt()) {
                bidders.push_back(&player);
            }
        }
    }

    if (bidders.empty()) {
        property->resetOwnerAsBank();
        view.outputNoBid(property);
        return;
    }

    auto findBidderIndex = [&](Player *player) {
        for (size_t i = 0; i < bidders.size(); ++i) {
            if (bidders[i] == player) {
                return i;
            }
        }
        return static_cast<size_t>(0);
    };

    if (excludedPlayer == nullptr) {
        Player *startFrom = &getCurrentPlayer();
        for (size_t offset = 0; offset <= players.size(); ++offset) {
            auto playerIt = std::find_if(players.begin(), players.end(), [&](const Player &player) {
                return &player == startFrom;
            });
            if (playerIt == players.end()) {
                break;
            }
            size_t playerIndex = static_cast<size_t>(std::distance(players.begin(), playerIt));
            Player *candidate = &players[(playerIndex + offset) % players.size()];
            if (std::find(bidders.begin(), bidders.end(), candidate) != bidders.end()) {
                currentBidderIndex = findBidderIndex(candidate);
                break;
            }
        }
    }

    Player *lastBidder = nullptr;
    long long bestBidAmount = -1;
    std::vector<bool> activeBidders(bidders.size(), true);
    int activeBidderCount = static_cast<int>(bidders.size());
    int activeOpeningBidderCount = static_cast<int>(std::count_if(bidders.begin(), bidders.end(), [&](Player *bidder) {
        return bidder != excludedPlayer;
    }));

    while (activeBidderCount > 0) {
        if (lastBidder != nullptr && activeBidderCount == 1) {
            break;
        }

        Player *currentBidder = bidders[currentBidderIndex];
        if (!activeBidders[currentBidderIndex] || currentBidder == lastBidder) {
            currentBidderIndex = (currentBidderIndex + 1) % bidders.size();
            continue;
        }

        bool canPass = lastBidder != nullptr;
        if (lastBidder == nullptr) {
            canPass = excludedPlayer == nullptr ? activeBidderCount > 1 : activeOpeningBidderCount > 1;
        }
        long long currentBidAmount = view.promptBidOrPass(*currentBidder, bestBidAmount, lastBidder, activeBidderCount, canPass);
        if (currentBidAmount >= 0) {
            lastBidder = currentBidder;
            bestBidAmount = currentBidAmount;
            view.outputBidAccepted(*currentBidder, currentBidAmount);
            logger.log(turn, currentBidder->getUsername(), "BID",
                       "Melakukan bid terhadap " + property->getName() + " [ " + property->getCode() + "]" +
                           " sebanyak " + std::to_string(currentBidAmount));
        } else {
            activeBidders[currentBidderIndex] = false;
            activeBidderCount--;
            if (currentBidder != excludedPlayer) {
                activeOpeningBidderCount--;
            }
            view.outputPass(*currentBidder, activeBidderCount);
            logger.log(turn, currentBidder->getUsername(), "PASS",
                       "Melewati pelelangan " + property->getName() + " [ " + property->getCode() + "]");
        }
        currentBidderIndex = (currentBidderIndex + 1) % bidders.size();
    }

    if (lastBidder == nullptr) {
        property->resetOwnerAsBank();
        view.outputNoBid(property);
    } else {
        try {
            lastBidder->buyProperty(property, bestBidAmount);
            logger.log(turn, lastBidder->getUsername(), "BELI",
                       "Berhasil menang lelang. Properti " + property->getName() + " [" + property->getCode() + "] dibeli seharga M" + std::to_string(bestBidAmount));
            view.outputWinner(lastBidder, property, bestBidAmount);
        } catch (const PlayerException &e) {
            std::cout << e.what() << std::endl;
            view.outputNoBid(property);
        }
    }
}
void GameManager::processMortgageProperty() {
    Player &player = getCurrentPlayer();
    MortgageView &view = gameView.getMortgageView();
    Property *chosenProperty = view.promptChooseProperty(player.getProperties());
    if (chosenProperty == nullptr) return;

    if (chosenProperty->getPropertyType() == "STREET") {
        StreetProperty *chosenStreet = dynamic_cast<StreetProperty *>(chosenProperty);
        std::string color = chosenStreet->getColor();
        std::vector<StreetProperty *> colorGroupWithBuildings;
        bool hasBuildings = false;
        for (Property *pr : player.getProperties()) {
            if (pr->getColor() == color) {
                if (auto sp = dynamic_cast<StreetProperty *>(pr)) {
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
            for (StreetProperty *sp : colorGroupWithBuildings) {
                long long refund = 0;
                int count = sp->getHouseCount();
                bool hasHotel = sp->hasHotel();
                for (int i = 0; i < count; i++) {
                    refund += (sp->hasHotel() ? sp->getHotelPrice() : sp->getHousePrice()) / 2;
                    sp->removeBuilding();
                }
                if (hasHotel) {
                    logger.log(turn, player.getUsername(), "JUAL",
                               "Bangunan di properti " + sp->getName() + " [" + sp->getCode() + "] berupa sebuah hotel dijual seharga M" + std::to_string(refund));
                } else {
                    logger.log(turn, player.getUsername(), "JUAL",
                               "Bangunan di properti " + sp->getName() + " [" + sp->getCode() + "] berupa " + std::to_string(count) + " rumah dijual seharga M" + std::to_string(refund));
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
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        view.outputMortgageStatus(false, *chosenProperty);
    }
}
void GameManager::processUnmortgageProperty() {
    Player &player = getCurrentPlayer();
    UnmortgageView &view = gameView.getUnmortgageView();
    Property *property = view.promptChooseProperty(player.getProperties());
    if (property == nullptr) return;

    try {
        player.unmortgageProperty(property);
        logger.log(turn, player.getUsername(), "TEBUS",
                   property->getName() + " [ " + property->getCode() + "] ditebus seharga " + std::to_string(property->redemptionPrice()));
        view.outputUnmortgageStatus(true, *property);
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        view.outputUnmortgageStatus(false, *property);
    }
}
void GameManager::processBuild() {
    Player &player = getCurrentPlayer();

    BuildView &view = gameView.getBuildView();

    StreetProperty *street = view.promptChooseProperty(player.getProperties());
    if (street != nullptr) {
        try {
            long long buildPrice = street->getHouseCount() < 4 ? street->getHousePrice() : street->getHotelPrice();
            if (player.hasEffect("DISCOUNT")) {
                buildPrice = buildPrice * (100 - player.getEffectValue("DISCOUNT")) / 100;
            }
            if (street->getHouseCount() < 4) {
                player.buildOnProperty(street);
                logger.log(turn, player.getUsername(), "BANGUN",
                           street->getName() + "[" + street->getCode() + "] di-upgrade seharga M" + std::to_string(buildPrice) +
                               " dan sekarang memiliki " + std::to_string(street->getHouseCount()) + " rumah");
            } else {
                player.buildOnProperty(street);
                logger.log(turn, player.getUsername(), "BANGUN",
                           street->getName() + " di-upgrade menjadi hotel seharga M" + std::to_string(buildPrice));
            }
            view.outputBuildStatus(true, street);
        } catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
            view.outputBuildStatus(false, nullptr);
        }
    } else {
        view.outputBuildStatus(false, nullptr);
    }
}
void GameManager::processUseSkillCard() {
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
        if (!card->prepareUse(gameView.getUseSkillCardView(), *this)) {
            view.outputCardUseCancelled(*card);
            return;
        }
        player.useSkillCard(skillIndex, *this);
        skillCardUsedThisTurn = true;
        view.outputCardUseResult(*card);
        gameView.getMainMenuView().outputCurrentPlayerInfo();
        logger.log(turn, player.getUsername(), "USE_CARD",
                   card->getCardType() + " dipakai. " + card->getMessage());
    } catch (const PlayerException &e) {
        std::cout << e.what() << std::endl;
    }
}
void GameManager::processDropSkillCard() {
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
    } catch (const PlayerException &e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
    }
}
void GameManager::processLiquidation() {
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
                StreetProperty *sp = dynamic_cast<StreetProperty *>(chosenProperty.second);
                long long refund = 0;
                int count = sp->getHouseCount();
                bool hasHotel = sp->hasHotel();
                for (int i = 0; i < count; i++) {
                    refund += (sp->hasHotel() ? sp->getHotelPrice() : sp->getHousePrice()) / 2;
                }
                if (hasHotel) {
                    logger.log(turn, player.getUsername(), "JUAL",
                               "Bangunan di properti " + sp->getName() + " [" + sp->getCode() + "] berupa sebuah hotel dijual seharga M" + std::to_string(refund));
                } else {
                    logger.log(turn, player.getUsername(), "JUAL",
                               "Bangunan di properti " + sp->getName() + " [" + sp->getCode() + "] berupa " + std::to_string(count) + " rumah dijual seharga M" + std::to_string(refund));
                }
                player.sellProperty(chosenProperty.second);
            } else if (chosenProperty.first == "Gadai") {
                logger.log(turn, player.getUsername(), "GADAI",
                           chosenProperty.second->getName() + " [" + chosenProperty.second->getCode() + "]" +
                               " digadaikan seharga " + std::to_string(chosenProperty.second->getMortgageValue()));
                player.mortgageProperty(chosenProperty.second);
            }
        }
        // Deduct debt from player (tax goes to bank and is removed from circulation)
        player.deductMoney(debt);
    } else {
        view.outputBankruptByBank(player);
        std::vector<Property *> auctionedProperty = player.getProperties();
        player.bankruptByBank();
        logger.log(turn, player.getUsername(), "BANKRUPT", "Bangkrut oleh bank. Semua aset dilelangkan.");
        for (Property *property : auctionedProperty) {
            processAuctionProperty(property);
        }
    }
}
void GameManager::processOtherPlayerLiquidation(Player &other) {
    BankruptView &view = gameView.getBankruptView();

    long long debt = -other.getMoney();
    view.outputPotentialWealth(other, -other.getMoney());

    if (other.calculateTotalWealth() >= -other.getMoney()) {
        while (other.getMoney() < 0) {
            auto chosenProperty = view.promptLiquidation(other.getProperties(), -other.getMoney());
            if (chosenProperty.first == "Jual") {
                StreetProperty *sp = dynamic_cast<StreetProperty *>(chosenProperty.second);
                long long refund = 0;
                int count = sp->getHouseCount();
                bool hasHotel = sp->hasHotel();
                for (int i = 0; i < count; i++) {
                    refund += (sp->hasHotel() ? sp->getHotelPrice() : sp->getHousePrice()) / 2;
                }
                if (hasHotel) {
                    logger.log(turn, other.getUsername(), "JUAL",
                               "Bangunan di properti " + sp->getName() + " [" + sp->getCode() + "] berupa sebuah hotel dijual seharga M" + std::to_string(refund));
                } else {
                    logger.log(turn, other.getUsername(), "JUAL",
                               "Bangunan di properti " + sp->getName() + " [" + sp->getCode() + "] berupa " + std::to_string(count) + " rumah dijual seharga M" + std::to_string(refund));
                }
                other.sellProperty(chosenProperty.second);
            } else if (chosenProperty.first == "Gadai") {
                logger.log(turn, other.getUsername(), "GADAI",
                           chosenProperty.second->getName() + " [" + chosenProperty.second->getCode() + "]" +
                               " digadaikan seharga " + std::to_string(chosenProperty.second->getMortgageValue()));
                other.mortgageProperty(chosenProperty.second);
            }
        }
        other.deductMoney(debt);
    } else {
        view.outputBankruptByBank(other);
        std::vector<Property *> auctionedProperty = other.getProperties();
        other.bankruptByBank();
        logger.log(turn, other.getUsername(), "BANKRUPT", "Bangkrut oleh bank. Semua aset dilelangkan.");
        for (Property *property : auctionedProperty) {
            processAuctionProperty(property);
        }
    }
}
void GameManager::processOtherPlayerLiquidation(Player &other, Player &creditor) {
    BankruptView &view = gameView.getBankruptView();

    long long debt = -other.getMoney();
    view.outputPotentialWealth(other, -other.getMoney());

    if (other.calculateTotalWealth() >= -other.getMoney()) {
        while (other.getMoney() < 0) {
            auto chosenProperty = view.promptLiquidation(other.getProperties(), -other.getMoney());
            if (chosenProperty.first == "Jual") {
                other.sellProperty(chosenProperty.second);
            } else if (chosenProperty.first == "Gadai") {
                other.mortgageProperty(chosenProperty.second);
            }
        }
        other.deductMoney(debt);
        creditor.receiveMoney(debt);
        view.outputDebtPaid(debt, &creditor);
    } else {
        view.outputBankruptByPlayer(other, creditor);
        std::vector<Property *> auctionedProperty = other.getProperties();
        logger.log(turn, other.getUsername(), "BANKRUPT", "Bangkrut oleh player " + creditor.getUsername() + " . Semua aset dialihkan untuk kreditor.");
        for (Property *property : auctionedProperty) {
            logger.log(turn, other.getUsername(), "AMBIL_ALIH", "Properti " + property->getName() + "[" + property->getCode() + "] dialihkan menjadi milik " + creditor.getUsername());
        }
        other.bankruptByPlayer(&creditor);
    }
}
void GameManager::processLiquidation(Player &creditor) {
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
            } else if (chosenProperty.first == "Gadai") {
                player.mortgageProperty(chosenProperty.second);
            }
        }
        player.deductMoney(debt);
        creditor.receiveMoney(debt);
        view.outputDebtPaid(debt, &creditor);
    } else {
        view.outputBankruptByPlayer(player, creditor);
        std::vector<Property *> auctionedProperty = player.getProperties();
        logger.log(turn, player.getUsername(), "BANKRUPT", "Bangkrut oleh player " + creditor.getUsername() + " . Semua aset dialihkan untuk kreditor.");
        for (Property *property : auctionedProperty) {
            logger.log(turn, player.getUsername(), "AMBIL_ALIH", "Properti " + property->getName() + "[" + property->getCode() + "] dialihkan menjadi milik " + creditor.getUsername());
        }
        player.bankruptByPlayer(&creditor);
    }
}

void GameManager::processWin() {
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

    logger.log(turn, winner->getUsername(), "MENANG", "Berhasil memenangkan permainan.");
    playing = false;
    stopGame();
}

void GameManager::processPayRent() {
    Player &player = getCurrentPlayer();
    processPayRent(player);
}

void GameManager::processPayRent(Player &player) {
    PlayerPiece &piece = player.getPiece();

    PropertyTile *tile = dynamic_cast<PropertyTile *>(piece.getCurrentTile());
    Property *prop = tile->getProperty();
    PropertyView &propView = gameView.getPropertyView();
    propView.outputRent(*prop, player);
    if (prop->getState() == PropertyState::MORTGAGED) {
        return;
    }
    int pay = player.payRent(tile->getProperty());
    long long rent = tile->getProperty()->calculateRent();
    if (player.hasEffect("DISCOUNT")) {
        rent = rent * (100 - player.getEffectValue("DISCOUNT")) / 100;
        logger.log(turn, player.getUsername(), "SEWA", "Bayar sewa di properti " + tile->getProperty()->getName() + "[" + tile->getProperty()->getCode() + "] sebesar M" + std::to_string(rent) + " (diskon sebesar " + std::to_string(player.getEffectValue("DISCOUNT")) + "%)");
    } else {
        logger.log(turn, player.getUsername(), "SEWA", "Bayar sewa di properti " + tile->getProperty()->getName() + "[" + tile->getProperty()->getCode() + "] sebesar M" + std::to_string(rent));
    }
    if (!pay) {
        if (&player == &getCurrentPlayer()) {
            processLiquidation(*prop->getOwner());
        } else {
            processOtherPlayerLiquidation(player, *prop->getOwner());
        }
    }
}

void GameManager::processGoTile() {
    Player &player = getCurrentPlayer();
    BoardView &board = gameView.getBoardView();
    board.outputOnPassByStart();
    logger.log(turn, player.getUsername(), "GO", "Melewati petak GO. Mendapat uang sebesar M" + std::to_string(config.goSalary));
}

// FIX: kept processLandingMessage from HEAD - used to print landing output for current tile
void GameManager::processLandingMessage() {
    gameView.getBoardView().outputOnLanded();
}

void GameManager::processGoToJail(const std::string &reason) {
    processGoToJail(getCurrentPlayer(), reason);
}

void GameManager::processGoToJail(Player &player, const std::string &reason) {
    JailView &jail = gameView.getJailView();
    jail.outputGoToJail(player, reason);
    logger.log(turn, player.getUsername(), "JAIL", "Dipenjara karena: " + reason);
}

void GameManager::processPayLuxuryTax() {
    processPayLuxuryTax(getCurrentPlayer());
}

void GameManager::processPayLuxuryTax(Player &player) {
    TaxView &taxView = gameView.getTaxView();
    taxView.outputLuxuryTax(config.luxuryFlatTax, player);

    bool payTax = player.payTax(config.luxuryFlatTax);
    logger.log(turn, player.getUsername(), "PAJAK", "Membayar pajak barang mewah sebesar M" + std::to_string(config.luxuryFlatTax));
    if (!payTax) {
        if (&player == &getCurrentPlayer()) {
            processLiquidation();
        } else {
            processLiquidation(player);
        }
    }
}

void GameManager::processPayIncomeTax() {
    processPayIncomeTax(getCurrentPlayer());
}

void GameManager::processPayIncomeTax(Player &player) {
    TaxView &taxView = gameView.getTaxView();
    int input = taxView.promptIncomeTax(config.incomeFlatTax, config.incomePercentageTax, player);
    long long amount;
    if (input == 1) {
        amount = config.incomeFlatTax;
    } else if (input == 2) {
        amount = player.calculateTotalWealth() * config.incomePercentageTax / 100;
    } else {
        return;
    }
    logger.log(turn, player.getUsername(), "PAJAK", "Membayar pajak penghasilan sebesar M" + std::to_string(amount));
    bool payTax = player.payTax(amount);
    if (!payTax) {
        if (&player == &getCurrentPlayer()) {
            processLiquidation();
        } else {
            processLiquidation(player);
        }
    }
}

void GameManager::processUseCommunityChestCard() {
    processUseCommunityChestCard(getCurrentPlayer());
}

void GameManager::processUseCommunityChestCard(Player &p) {
    CardView &cardView = gameView.getCardView();
    CommunityChestCard *card = communityChestCardDeck.drawCard();
    cardView.outputCard(*card);
    logger.log(turn, p.getUsername(), "DANA_UMUM", "Mendapat kartu dana umum. " + card->getMessage());
    if ((card->getCardType() == "PAYMONEYCARD" || card->getCardType() == "PAYMONEYTOPLAYERSCARD") && p.hasEffect("SHIELD")) {
        if (p.hasEffect("SHIELD")) {
            cardView.outputShielded();
            return;
        }
    }
    card->takeEffect(p, *this);
}

void GameManager::processUseChanceCard() {
    processUseChanceCard(getCurrentPlayer());
}

void GameManager::processUseChanceCard(Player &p) {
    CardView &cardView = gameView.getCardView();
    ChanceCard *card = chanceCardDeck.drawCard();
    cardView.outputCard(*card);
    logger.log(turn, p.getUsername(), "DANA_UMUM", "Mendapat kartu kesempatan. " + card->getMessage());
    if (card->getCardType() == "GOTOJAILCARD") {
        processGoToJail(p, "Kartu Chance memerintahkan kamu masuk penjara.");
    }
    card->takeEffect(p, *this);
}

void GameManager::processStartFestival() {
    processStartFestival(getCurrentPlayer());
}

void GameManager::processStartFestival(Player &p) {
    FestivalView &fesView = gameView.getFestivalView();
    std::vector<Property *> properties = p.getProperties();
    if (properties.empty()) {
        std::cout << "Kamu tidak memiliki properti. Efek festival tidak dapat digunakan.\n\n";
        return;
    }
    Property *prop = fesView.promptChooseProperty(properties);
    if (prop == nullptr) return;
    long long previousMultiplier = prop->getFestivalMultiplier();
    prop->startFestival();
    logger.log(turn, p.getUsername(), "FESTIVAL", "Memulai festival di " + prop->getName() + " [ " + prop->getCode() + "]");
    fesView.outputFestivalStatus(*prop, previousMultiplier);
}
void GameManager::processExit() {
    logger.log(turn, "GAME", "EXIT", "Keluar dari permainan.");
    running = false;
    playing = false;
}
