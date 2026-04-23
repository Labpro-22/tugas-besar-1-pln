#include "utils/save-load/SaveFileHandler.hpp"

void SaveFileHandler::saveGame(SaveData saveData, std::string fileName)
{
    std::filesystem::path path = "data/" + fileName + ".txt";
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
    std::filesystem::path path = "data/" + fileName + ".txt";
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
        PlayerSaveData player;

        int col = 0;
        line++;
        std::getline(in, buffer);
        bufferStream = std::stringstream(buffer);
        col++;
        if (!(bufferStream >> player.username)) {
            throw SaveFileFormatException("USERNAME", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> player.money)) {
            throw SaveFileFormatException("UANG", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> player.tileCodePosition)) {
            throw SaveFileFormatException("POSISI_PETAK", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> player.getOutOfJailCardCount)) {
            player.getOutOfJailCardCount = 0;
        }
        col++;
        if (!(bufferStream >> player.jailTurns)) {
            player.jailTurns = 0;
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
            col++;
            if (!(bufferStream >> skillCardCount)) {
                throw SaveFileFormatException("JENIS_KARTU", path.string(), col, line);
            }
            col++;
            if (!(bufferStream >> skillCardCount)) {
                throw SaveFileFormatException("NILAI_KARTU", path.string(), col, line);
            }
            col++;
            if (!(bufferStream >> skillCardCount)) {
                throw SaveFileFormatException("SISA_DURASI", path.string(), col, line);
            }
        }

        saveData.players.push_back(player);
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
        PropertySaveData property;
        line++;
        std::getline(in, buffer);
        std::stringstream bufferStream(buffer);
        int col = 0;
        col++;
        if (!(bufferStream >> property.code)) {
            throw SaveFileFormatException("KODE", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> property.type)) {
            throw SaveFileFormatException("KODE", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> property.owner)) {
            throw SaveFileFormatException("KODE", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> property.status)) {
            throw SaveFileFormatException("KODE", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> property.festivalMultiplier)) {
            throw SaveFileFormatException("KODE", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> property.festivalDuration)) {
            throw SaveFileFormatException("KODE", path.string(), col, line);
        }
        std::string temp;
        col++;
        if (!(bufferStream >> temp)) {
            throw SaveFileFormatException("KODE", path.string(), col, line);
        }
        if (temp == "H") {
            property.houseCount = 0;
            property.hasHotel = true;
        }
        else {
            property.houseCount = std::stoi(temp);
            property.hasHotel = false;
        }
        saveData.properties.push_back(property);
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
        LogSaveData log;
        line++;
        std::getline(in, buffer);
        std::stringstream bufferStream(buffer);
        int col = 0;
        col++;
        if (!(bufferStream >> log.turn)) {
            throw SaveFileFormatException("TURN", path.string(), 1, line);
        }
        col++;
        if (!(bufferStream >> log.username)) {
            throw SaveFileFormatException("USERNAME", path.string(), 1, line);
        }
        col++;
        if (!(bufferStream >> log.action)) {
            throw SaveFileFormatException("JENIS_AKSI", path.string(), 1, line);
        }
        col++;
        if (!(std::getline(bufferStream, log.details))) {
            throw SaveFileFormatException("DETAIL", path.string(), 1, line);
        }
        saveData.logs.push_back(log);
    }
}