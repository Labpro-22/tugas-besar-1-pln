#include "core/GameManager.hpp"
#include "views/BoardView.hpp"
#include "models/tile/PropertyTile.hpp"

#include "models/tile/special-tile/JailTile.hpp"
#define TILE_WIDTH 10
#define CELL_WIDTH 10
#define TILE_HEIGHT 2
void BoardView::outputOnLanded()const{
    Player &p = gameManager.getCurrentPlayer();
    Tile* t = p.getPiece().getCurrentTile();
    if(auto pt = dynamic_cast<PropertyTile*>(t)){
        std::cout << "Kamu mendarat di " << pt->getName() << "(" + pt->getProperty()->getCode() + ")";
        Player *owner = pt->getProperty()->getOwner();
        if( owner != nullptr){
            if(owner->getUsername() != p.getUsername()){
                std::cout <<", milik Pemain " << owner->getUsername();
            }
            else {std::cout <<", milikmu";}
        }
    }
    else{
        std::cout << "Kamu mendarat di petak " << t->getName();
    }
    std::cout << "!\n\n";
}
void BoardView::outputOnLanded(Player &p)const{
    Tile* t = p.getPiece().getCurrentTile();
    if(auto pt = dynamic_cast<PropertyTile*>(t)){
        std::cout << "Pemain " + p.getUsername() <<" mendarat di " << pt->getName() << "(" + pt->getProperty()->getCode() + ")";
        Player *owner = pt->getProperty()->getOwner();
        if( owner != nullptr){
            if(owner->getUsername() != p.getUsername()){
                std::cout <<", milik Pemain " << owner->getUsername();
            }
        }
    }
    else{
        std::cout << "Pemain " + p.getUsername()<<  " mendarat di petak " << t->getName();
    }
    std::cout << "!\n\n";
}

void BoardView::outputOnPassByStart()const{
    std::cout << "Kamu melewati GO.\n";
    std::cout << "Kamu mendapatkan M" << gameManager.getConfig().goSalary << '\n';
}

std::string BoardView::getColorCode(const std::string& color) const{
    if (color == "COKLAT")      return BROWN;
    if (color == "BIRU_MUDA")   return LIGHT_BLUE;
    if (color == "MERAH_MUDA")  return PINK;
    if (color == "ORANGE")      return ORANGE;
    if (color == "MERAH")       return RED;
    if (color == "KUNING")      return YELLOW;
    if (color == "HIJAU")       return GREEN;
    if (color == "BIRU_TUA")    return DARK_BLUE;
    if (color == "ABU_ABU")     return GREY ;
    return DEFAULT;
}

std::string BoardView::resetColor() const{
    return "\033[0m";
}

std::string BoardView::getColorTag(const std::string& color)const {
    if (color == "COKLAT")      return "CK";
    if (color == "BIRU_MUDA")   return "BM";
    if (color == "MERAH_MUDA")  return "PK";
    if (color == "ORANGE")      return "OR";
    if (color == "MERAH")       return "MR";
    if (color == "KUNING")      return "KN";
    if (color == "HIJAU")       return "HJ";
    if (color == "BIRU_TUA")    return "BT";
    if (color == "ABU_ABU")     return "AB";
    return "DF";
}

std::string BoardView::padRight(const std::string& str, int width) const{
    if ((int)str.size() >= width) return str.substr(0, width);
    return str + std::string(width - str.size(), ' ');
}

std::string BoardView::padCenter(const std::string& str, int width) const{
    if ((int)str.size() >= width) return str.substr(0, width);
    int padding = width - str.size();
    int left = padding / 2;
    int right = padding - left;
    return std::string(left, ' ') + str + std::string(right, ' ');
}

std::string BoardView::getPlayersAtPosition(int position)const {
    std::string result = "";
    auto& players = gameManager.getPlayers();
    for (size_t i = 0; i < players.size(); i++) {
        if (players[i].getState() == PlayerState::BANKRUPT) continue;
        if (players[i].getPiece().getPosition() == position) {
            if (!result.empty()) result += " ";
            result += "(" + std::to_string(i + 1) + ")";
        }
    }
    return result;
}

std::string BoardView::getOwnershipInfo(Property* pr) const{
    if (pr == nullptr) return "";
    
    std::string info = "";
    if (pr->getOwner() != nullptr) {
        auto& players = gameManager.getPlayers();
        for (size_t i = 0; i < players.size(); i++) {
            if (&players[i] == pr->getOwner()) {
                info += "P" + std::to_string(i + 1);
                break;
            }
        }
        StreetProperty* street = dynamic_cast<StreetProperty*>(pr);
        if (street) {
            int houses = street->getHouseCount();
            if (houses == 5) {
                info += " *";
            } else {
                for (int j = 0; j < houses; j++) {
                    info += "^";
                }
            }
        }
        if (pr->isMortgaged()) {
            info += " [M]";
        }
    }
    return info;
}

static std::string getTileColor(Tile* tile, const BoardView& bv) {
    auto* pt = dynamic_cast<PropertyTile*>(tile);
    if (pt && pt->getProperty()) {
        std::string col = pt->getProperty()->getColor();
        if (col != "DEFAULT") return bv.getColorCode(col);
    }
    return "";
}

std::string BoardView::getCellLine1(Tile* tile, int) const{
    if (tile == nullptr) {
        return padRight("[DF] ???", CELL_WIDTH);
    }

    std::string colorTag = "DF";
    std::string code = tile->getCode();

    PropertyTile* propTile = dynamic_cast<PropertyTile*>(tile);
    if (propTile && propTile->getProperty()) {
        colorTag = getColorTag(propTile->getProperty()->getColor());
    }

    std::string raw = "[" + colorTag + "] " + code;
    std::string padded = padRight(raw, CELL_WIDTH);

    std::string colorCode = getTileColor(tile, *this);
    if (!colorCode.empty()) {
        return colorCode + padded + resetColor();
    }
    return padded;
}

std::string BoardView::getCellLine2(Tile* tile, int tileIndex) const{
    if (tile == nullptr) {
        return "";
    }

    std::string info = "";

    PropertyTile* propTile = dynamic_cast<PropertyTile*>(tile);
    if (propTile && propTile->getProperty()) {
        info += getOwnershipInfo(propTile->getProperty());
    }

    std::string playersHere = getPlayersAtPosition(tileIndex);
    if (!playersHere.empty()) {
        if (!info.empty()) info += " ";
        info += playersHere;
    }

    JailTile* jail = dynamic_cast<JailTile*>(tile);
    if (jail) {
        std::string inList, visitList;
        auto& players = gameManager.getPlayers();
        for (int i = 0; i < (int)players.size(); i++) {
            if (players[i].getPiece().getPosition() == tileIndex) {
                std::string tag = "(" + std::to_string(i + 1) + ")";
                if (players[i].isJailed()) inList += tag;
                else visitList += tag;
            }
        }
        if (!inList.empty() || !visitList.empty()) {
            info = "IN:" + (inList.empty() ? "-" : inList) +
                   " V:" + (visitList.empty() ? "-" : visitList);
        }
    }

    std::string padded = padRight(info, CELL_WIDTH);
    std::string colorCode = getTileColor(tile, *this);
    if (!colorCode.empty()) {
        return colorCode + padded + resetColor();
    }
    return padded;
}

void BoardView::printHorizontalBorder(int count) const{
    for (int i = 0; i < count; i++) {
        std::cout << "+";
        for (int j = 0; j < CELL_WIDTH; j++) std::cout << "-";
    }
    std::cout << "+" << std::endl;
}

std::string BoardView::getCenterContent(int row) const{
    int turn = gameManager.getCurrentTurn();
    int maxTurn = gameManager.getConfig().maxTurn;

    Board& board = gameManager.getBoard();
    int sideLength = board.getTileCount() / 4;

    int innerCols = sideLength - 1;
    int innerWidth = (CELL_WIDTH + 1) * innerCols - 1;

    auto c = [&](const std::string& s) { return padCenter(s, innerWidth); };

    auto& players = gameManager.getPlayers();
    auto playerLabel = [&](int idx) -> std::string {
        if (idx >= (int)players.size()) return "";
        return "P" + std::to_string(idx + 1) + " = " + players[idx].getUsername();
    };
    auto playerLine = [&](int a, int b) -> std::string {
        std::string la = playerLabel(a);
        std::string lb = playerLabel(b);
        if (la.empty() && lb.empty()) return "";
        if (lb.empty()) return la;
        return padRight(la, 18) + "  " + lb;
    };

    switch (row) {
        case  0:  return c("");
        case  1:  return c("==========================================");
        case  2:  return c("||         N I M O N S P O L I          ||");
        case  3:  return c("==========================================");
        case  4:  return c("");
        case  5:  return c("[ TURN " + std::to_string(turn) + " / " + std::to_string(maxTurn) + " ]");
        case  6:  return c("");
        case  7:  return c("---------------- PEMAIN ----------------");
        case  8:  return c(playerLine(0, 1));
        case  9:  return c(playerLine(2, 3));
        case 10:  return c("------ LEGENDA KEPEMILIKAN & STATUS ------");
        case 11:  return c("P1-P4    : Properti milik Pemain 1-4");
        case 12:  return c("^ ^^ ^^^ ^^^^ : Rumah Level 1-4");
        case 13:  return c("*        : Hotel (Maksimal)");
        case 14:  return c("[M]      : Properti Digadaikan");
        case 15:  return c("(1)-(4)  : Bidak Pemain di Petak");
        case 16:  return c("IN / V   : Tahanan / Mampir di Penjara");
        case 17:  return c("--------------- KODE WARNA ---------------");
        case 18:  return c("[CK]=Coklat      [MR]=Merah");
        case 19:  return c("[BM]=Biru Muda   [KN]=Kuning");
        case 20:  return c("[PK]=Pink        [HJ]=Hijau");
        case 21:  return c("[OR]=Orange      [BT]=Biru Tua");
        case 22:  return c("[AB]=Utilitas    [DF]=Aksi/Stasiun");
        default:  return c("");
    }
}

void BoardView::printTopRow() const{
    Board& board = gameManager.getBoard();
    int size = board.getTileCount();
    int sideLength = size / 4;
    int topCount = sideLength + 1;

    printHorizontalBorder(topCount);
    for (int i = 0; i < topCount; i++) {
        int tileIndex = sideLength * 2 + i;
        Tile* tile = board.getTile(tileIndex);
        std::cout << "|" << getCellLine1(tile, tileIndex);
    }
    std::cout << "|" << std::endl;

    for (int i = 0; i < topCount; i++) {
        int tileIndex = sideLength * 2 + i;
        Tile* tile = board.getTile(tileIndex);
        std::cout << "|" << getCellLine2(tile, tileIndex);
    }
    std::cout << "|" << std::endl;

    printHorizontalBorder(topCount);
}

void BoardView::printBottomRow() const{
    Board &board = gameManager.getBoard();
    int sideLength = board.getTileCount() / 4;
    int bottomCount = sideLength + 1;
    printHorizontalBorder(bottomCount);

    for (int i = 0; i < bottomCount; i++) {
        int tileIndex = sideLength - i;
        Tile* tile = board.getTile(tileIndex);
        std::cout << "|" << getCellLine1(tile, tileIndex);
    }
    std::cout << "|" << std::endl;

    for (int i = 0; i < bottomCount; i++) {
        int tileIndex = sideLength - i;
        Tile* tile = board.getTile(tileIndex);
        std::cout << "|" << getCellLine2(tile, tileIndex);
    }
    std::cout << "|" << std::endl;

    printHorizontalBorder(bottomCount);
}

void BoardView::printMiddleRows()const {
    Board &board = gameManager.getBoard();
    int sideLength = board.getTileCount() / 4;
    int middleCount = sideLength - 1;

    auto cellSeparator = [&](const std::string& centerLine) {
        std::cout << "+";
        for (int j = 0; j < CELL_WIDTH; j++) std::cout << "-";
        std::cout << "+";
        std::cout << centerLine;
        std::cout << "+";
        for (int j = 0; j < CELL_WIDTH; j++) std::cout << "-";
        std::cout << "+" << std::endl;
    };

    for (int row = 0; row < middleCount; row++) {
        int leftIndex  = sideLength * 2 - 1 - row;
        int rightIndex = sideLength * 3 + 1 + row;

        Tile* leftTile  = board.getTile(leftIndex);
        Tile* rightTile = board.getTile(rightIndex);

        int slot0 = row * 3;
        int slot1 = row * 3 + 1;
        int slot2 = row * 3 + 2;

        std::cout << "|" << getCellLine1(leftTile, leftIndex);
        std::cout << "|" << getCenterContent(slot0);
        std::cout << "|" << getCellLine1(rightTile, rightIndex);
        std::cout << "|" << std::endl;

        std::cout << "|" << getCellLine2(leftTile, leftIndex);
        std::cout << "|" << getCenterContent(slot1);
        std::cout << "|" << getCellLine2(rightTile, rightIndex);
        std::cout << "|" << std::endl;

        if (row < middleCount - 1) {
            cellSeparator(getCenterContent(slot2));
        }
    }
}

void BoardView::drawBoard() const{
    std::cout << std::endl;
    printTopRow();
    printMiddleRows();
    printBottomRow();
    std::cout << std::endl;
}
