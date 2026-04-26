#include "utils/save-load/SaveFileHandler.hpp"

void SaveFileHandler::saveGame(SaveData saveData, std::string fileName)
{
    std::filesystem::path path = "data/" + fileName;
    std::filesystem::create_directories(path.parent_path());

    std::ofstream out(path);
    if (!out.is_open()) {
        throw SaveFileNotFoundException(path.string());
    }

    out << saveData.turn << " " << saveData.maxTurn << "\n";
    savePlayer(saveData, out);
    saveProperty(saveData, out);
    saveDeck(saveData, out);
    saveLog(saveData, out);
}

void SaveFileHandler::savePlayer(SaveData &saveData, std::ofstream &out)
{
    out << saveData.players.size() << "\n";
    for (PlayerSaveData &player : saveData.players) {
        out << player.username << " "
            << player.money << " "
            << player.tileCodePosition << " "
            << player.status << " "
            << player.getOutOfJailCardCount << " "
            << player.jailTurns << "\n";
        out << player.skillCards.size() << "\n";
        for (SkillCardSaveData &card : player.skillCards) {
            if (card.type == "DiscountCard") {
                out << card.type << " " << card.value << " " << card.duration << "\n";
            }
            else {
                out << card.type << " " << card.value << "\n";
            }
        }
    }
    for (std::string &player : saveData.playerOrder) {
        out << player << " ";
    }
    out << "\n";
    out << saveData.currentPlayer << "\n";
}

void SaveFileHandler::saveProperty(SaveData &saveData, std::ofstream &out)
{
    out << saveData.properties.size() << "\n";
    for (PropertySaveData &property : saveData.properties) {
        out << property.code << " "
            << property.type << " "
            << property.owner << " "
            << property.status << " "
            << property.festivalMultiplier << " "
            << property.festivalDuration << " "
            << (property.hasHotel ? std::to_string(property.houseCount) : "H") << "\n";
    }
}

void SaveFileHandler::saveDeck(SaveData &saveData, std::ofstream &out)
{
    out << saveData.deckCards.size() << "\n";
    for (std::string &card : saveData.deckCards) {
        out << card << "\n";
    }
}

void SaveFileHandler::saveLog(SaveData &saveData, std::ofstream &out)
{
    out << saveData.logs.size() << "\n";
    for (LogSaveData &log : saveData.logs) {
        out << log.turn << " " << log.username << " " << log.action << " " << log.details << "\n";
    }
}

SaveData SaveFileHandler::loadGame(std::string fileName)
{
    std::filesystem::path path = "data/" + fileName;
    std::ifstream in(path);
    if (!in.is_open()) {
        throw SaveFileNotFoundException(path.string());
    }

    int line = 0;
    SaveData saveData;

    line++;
    std::string buffer;
    std::getline(in, buffer);
    std::stringstream bufferStream(buffer);
    if (!(bufferStream >> saveData.turn)) {
        throw SaveFileFormatException("TURN", path.string(), 1, line);
    }
    if (!(bufferStream >> saveData.maxTurn)) {
        throw SaveFileFormatException("MAX_TURN", path.string(), 2, line);
    }
    loadPlayer(saveData, in, path, line);
    loadProperty(saveData, in, path, line);
    loadDeck(saveData, in, path, line);
    loadLog(saveData, in, path, line);

    return saveData;
}

void SaveFileHandler::loadPlayer(SaveData &saveData, std::ifstream &in, std::filesystem::path &path, int &line)
{
    std::string buffer;

    line++;
    std::getline(in, buffer);
    std::stringstream bufferStream(buffer);
    int playerCount;
    if (!(bufferStream >> playerCount)) {
        throw SaveFileFormatException("JUMLAH_PLAYER", path.string(), 1, line);
    }

    for (int i = 0; i < playerCount; i++) {
        PlayerSaveData playerData;

        int col = 0;
        line++;
        std::getline(in, buffer);
        bufferStream = std::stringstream(buffer);
        col++;
        if (!(bufferStream >> playerData.username)) {
            throw SaveFileFormatException("USERNAME", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> playerData.money)) {
            throw SaveFileFormatException("UANG", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> playerData.tileCodePosition)) {
            throw SaveFileFormatException("POSISI_PETAK", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> playerData.getOutOfJailCardCount)) {
            playerData.getOutOfJailCardCount = 0;
        }
        col++;
        if (!(bufferStream >> playerData.jailTurns)) {
            playerData.jailTurns = 0;
        }

        int skillCardCount;
        line++;
        std::getline(in, buffer);
        bufferStream = std::stringstream(buffer);
        col = 0;
        if (!(bufferStream >> skillCardCount)) {
            throw SaveFileFormatException("JUMLAH_KARTU_TANGAN", path.string(), col, line);
        }

        for (int i = 0; i < skillCardCount; i++) {
            col = 0;
            line++;
            std::getline(in, buffer);
            bufferStream = std::stringstream(buffer);

            SkillCardSaveData cardData;
            col++;
            if (!(bufferStream >> cardData.type)) {
                throw SaveFileFormatException("JENIS_KARTU", path.string(), col, line);
            }
            col++;
            if (!(bufferStream >> cardData.value)) {
                cardData.value = 0;
            }
            col++;
            if (!(bufferStream >> cardData.duration)) {
                cardData.duration = 1;
            }
            playerData.skillCards.push_back(cardData);
        }

        saveData.players.push_back(playerData);
    }
    line++;
    std::getline(in, buffer);
    bufferStream = std::stringstream(buffer);
    for (int i = 0; i < playerCount; i++) {
        std::string username;
        if (!(bufferStream >> username)) {
            throw SaveFileFormatException("URUTAN_GILIRAN_" + std::to_string(i + 1), path.string(), i, line);
        }
        saveData.playerOrder.push_back(username);
    }
    line++;
    std::getline(in, buffer);
    bufferStream = std::stringstream(buffer);
    if (!(bufferStream >> saveData.currentPlayer)) {
        throw SaveFileFormatException("GILIRAN_AKTIF_SAAT_INI", path.string(), 1, line);
    }
}

void SaveFileHandler::loadProperty(SaveData &saveData, std::ifstream &in, std::filesystem::path &path, int &line)
{
    std::string buffer;

    line++;
    std::getline(in, buffer);
    std::stringstream bufferStream(buffer);
    int propertyCount;
    if (!(bufferStream >> propertyCount)) {
        throw SaveFileFormatException("JUMLAH_PROPERTI", path.string(), 1, line);
    }
    for (int i = 0; i < propertyCount; i++) {
        PropertySaveData propertyData;
        line++;
        std::getline(in, buffer);
        std::stringstream bufferStream(buffer);
        int col = 0;
        col++;
        if (!(bufferStream >> propertyData.code)) {
            throw SaveFileFormatException("KODE", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> propertyData.type)) {
            throw SaveFileFormatException("TIPE", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> propertyData.owner)) {
            throw SaveFileFormatException("OWNER", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> propertyData.status)) {
            throw SaveFileFormatException("STATUS", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> propertyData.festivalMultiplier)) {
            throw SaveFileFormatException("FESTIVAL_MULTIPLIER", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> propertyData.festivalDuration)) {
            throw SaveFileFormatException("FESTIVAL_DURATION", path.string(), col, line);
        }
        std::string temp;
        col++;
        if (!(bufferStream >> temp)) {
            throw SaveFileFormatException("NBUILDING", path.string(), col, line);
        }
        if (temp == "H") {
            propertyData.houseCount = 0;
            propertyData.hasHotel = true;
        }
        else {
            propertyData.houseCount = std::stoi(temp);
            propertyData.hasHotel = false;
        }
        saveData.properties.push_back(propertyData);
    }
}

void SaveFileHandler::loadDeck(SaveData &saveData, std::ifstream &in, std::filesystem::path &path, int &line)
{
    std::string buffer;
    line++;
    std::getline(in, buffer);
    std::stringstream bufferStream(buffer);
    int cardCount;
    if (!(bufferStream >> cardCount)) {
        throw SaveFileFormatException("JUMLAH_KARTU", path.string(), 1, line);
    }
    for (int i = 0; i < cardCount; i++) {
        std::string card;
        line++;
        std::getline(in, buffer);
        std::stringstream bufferStream(buffer);
        if (!(bufferStream >> card)) {
            throw SaveFileFormatException("JENIS_KARTU", path.string(), 1, line);
        }
        saveData.deckCards.push_back(card);
    }
}

void SaveFileHandler::loadLog(SaveData &saveData, std::ifstream &in, std::filesystem::path &path, int &line)
{
    std::string buffer;
    line++;
    std::getline(in, buffer);
    std::stringstream bufferStream(buffer);
    int logCount;
    if (!(bufferStream >> logCount)) {
        throw SaveFileFormatException("JUMLAH_KARTU", path.string(), 1, line);
    }
    for (int i = 0; i < logCount; i++) {
        LogSaveData logData;
        line++;
        std::getline(in, buffer);
        std::stringstream bufferStream(buffer);
        int col = 0;
        col++;
        if (!(bufferStream >> logData.turn)) {
            throw SaveFileFormatException("TURN", path.string(), 1, line);
        }
        col++;
        if (!(bufferStream >> logData.username)) {
            throw SaveFileFormatException("USERNAME", path.string(), 1, line);
        }
        col++;
        if (!(bufferStream >> logData.action)) {
            throw SaveFileFormatException("JENIS_AKSI", path.string(), 1, line);
        }
        col++;
        if (!(std::getline(bufferStream, logData.details))) {
            throw SaveFileFormatException("DETAIL", path.string(), 1, line);
        }
        saveData.logs.push_back(logData);
    }
}