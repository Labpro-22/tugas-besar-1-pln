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
    if (color == "COKLAT")      return BROWN;       // kuning tua (approx coklat)
    if (color == "BIRU_MUDA")   return LIGHT_BLUE;       // cyan terang
    if (color == "MERAH_MUDA")  return PINK;       // magenta terang
    if (color == "ORANGE")      return ORANGE; // oranye
    if (color == "MERAH")       return RED;       // merah terang
    if (color == "KUNING")      return YELLOW;       // kuning terang
    if (color == "HIJAU")       return GREEN;       // hijau terang
    if (color == "BIRU_TUA")    return DARK_BLUE;       // biru terang
    if (color == "ABU_ABU")     return GREY ;      // abu-abu
    return DEFAULT; // default
}

std::string BoardView::resetColor() const{
    return "\033[0m";
}

// ==================== Helper: Tag Warna Singkat ====================

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
    return "DF"; // default untuk petak aksi/spesial
}

// ==================== Helper: Padding ====================

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

// ==================== Helper: Info Pemain di Petak ====================

std::string BoardView::getPlayersAtPosition(int position)const {
    std::string result = "";
    auto& players = gameManager.getPlayers();
    for (int i = 0; i < players.size(); i++) {
        if (players[i].getState() == PlayerState::BANKRUPT) continue;
        if (players[i].getPiece().getPosition() == position) {
            if (!result.empty()) result += " ";
            result += "(" + std::to_string(i + 1) + ")";
        }
    }
    return result;
}

// ==================== Helper: Info Kepemilikan ====================

std::string BoardView::getOwnershipInfo(Property* pr) const{
    if (pr == nullptr) return "";
    
    std::string info = "";
    if (pr->getOwner() != nullptr) {
        // Cari index pemain
        auto& players = gameManager.getPlayers();
        for (int i = 0; i < players.size(); i++) {
            if (&players[i] == pr->getOwner()) {
                info += "P" + std::to_string(i + 1);
                break;
            }
        }
    
        // Tampilkan level bangunan untuk street
        StreetProperty* street = dynamic_cast<StreetProperty*>(pr);
        if (street) {
            int houses = street->getHouseCount();
            if (houses == 5) {
                info += " *";       // hotel
            } else {
                for (int j = 0; j < houses; j++) {
                    info += "^";    // rumah
                }
            }
        }
    
        // Tandai jika digadaikan
        if (pr->isMortgaged()) {
            info += " [M]";
        }
    }
    return info;
}

// ==================== Sel: Baris 1 (tag warna + kode petak) ====================

std::string BoardView::getCellLine1(Tile* tile, int tileIndex) const{
    if (tile == nullptr) {
        return padRight("[DF] ???", CELL_WIDTH);
    }

    std::string colorTag = "DF";
    std::string code = tile->getCode();

    PropertyTile* propTile = dynamic_cast<PropertyTile*>(tile);
    if (propTile && propTile->getProperty()) {
        colorTag = getColorTag(propTile->getProperty()->getColor());
        std::string color = propTile->getProperty()->getColor();

        std::string raw = "[" + colorTag + "] " + code;
        std::string padded = padRight(raw, CELL_WIDTH);

        return getColorCode(color) + padded + resetColor();
    }

    return padRight("[" + colorTag + "] " + code, CELL_WIDTH);
}

// ==================== Sel: Baris 2 (kepemilikan + bidak) ====================

std::string BoardView::getCellLine2(Tile* tile, int tileIndex) const{
    if (tile == nullptr) {
        return "";
    }

    std::string info = "";

    // TODO: uncomment saat tersedia
    PropertyTile* propTile = dynamic_cast<PropertyTile*>(tile);
    if (propTile && propTile->getProperty()) {
        info += getOwnershipInfo(propTile->getProperty());
    }
    
    std::string playersHere = getPlayersAtPosition(tileIndex);
    if (!playersHere.empty()) {
        if (!info.empty()) info += " ";
        info += playersHere;
    }
    
    // Khusus penjara: tampilkan IN: dan V:
    JailTile* jail = dynamic_cast<JailTile*>(tile);
    if (jail) {
        int inCount = 0, visitCount = 0;
        auto& players = gameManager.getPlayers();
        for (auto& p : players) {
            if (p.getPiece().getPosition() == tileIndex) {
                if (p.isJailed()) inCount++;
                else visitCount++;
            }
        }
        if (inCount > 0 || visitCount > 0) {
            info = "IN:" + std::to_string(inCount) + " V:" + std::to_string(visitCount);
        }
    }

    return info;
}

// ==================== Border ====================

void BoardView::printHorizontalBorder(int count) const{
    for (int i = 0; i < count; i++) {
        std::cout << "+";
        for (int j = 0; j < CELL_WIDTH; j++) std::cout << "-";
    }
    std::cout << "+" << std::endl;
}

// ==================== Area Tengah ====================

std::string BoardView::getCenterContent(int row) const{
    int turn = gameManager.getCurrentTurn();
    int maxTurn = gameManager.getConfig().maxTurn;

    Board& board = gameManager.getBoard();
    int sideLength = board.getTileCount() / 4;

    int innerCols = sideLength - 1;
    int innerWidth = (CELL_WIDTH + 1) * innerCols - 1;

    auto c = [&](const std::string& s) { return padCenter(s, innerWidth); };

    // 9 middle rows × 3 slot per row = slot 0..26 (slot 26 tidak terpakai).
    // Slot 0,1 = baris konten sel; slot 2 = baris separator (juga jadi tempat teks).
    switch (row) {
        // Header
        case  0:  return c("");
        case  1:  return c("==========================================");
        case  2:  return c("||         N I M O N S P O L I          ||");
        case  3:  return c("==========================================");
        case  4:  return c("");
        case  5:  return c("[ TURN " + std::to_string(turn) + " / " + std::to_string(maxTurn) + " ]");
        case  6:  return c("");
        case  7:  return c("------- LEGENDA KEPEMILIKAN -------");

        // Legenda
        case  8:  return c("P1-P4    : Properti milik Pemain 1-4");
        case  9:  return c("^ ^^ ^^^ ^^^^ : Rumah Level 1-4");
        case 10:  return c("*        : Hotel (Maksimal)");
        case 11:  return c("[M]      : Properti Digadaikan");
        case 12:  return c("(1)-(4)  : Bidak Pemain di Petak");
        case 13:  return c("IN / V   : Tahanan / Mampir di Penjara");

        // Kode Warna
        case 14:  return c("");
        case 15:  return c("--------- KODE WARNA ---------");
        case 16:  return c("");
        case 17:  return c("[CK]=Coklat      [MR]=Merah");
        case 18:  return c("[BM]=Biru Muda   [KN]=Kuning");
        case 19:  return c("[PK]=Pink        [HJ]=Hijau");
        case 20:  return c("[OR]=Orange      [BT]=Biru Tua");
        case 21:  return c("[AB]=Utilitas    [DF]=Aksi/Stasiun");

        default:  return c("");
    }
}

// ==================== Cetak Baris Atas (index 20-30) ====================

void BoardView::printTopRow() const{
    // TODO: uncomment saat Board tersedia
    Board& board = gameManager.getBoard();
    int size = board.getTileCount();
    int sideLength = size / 4;  // papan dinamis support
    int topCount = sideLength + 1; // 11 petak di baris atas

    printHorizontalBorder(topCount);

    // Baris 1: tag warna + kode
    for (int i = 0; i < topCount; i++) {
        int tileIndex = sideLength * 2 + i;
        Tile* tile = board.getTile(tileIndex);
        std::cout << "|" << getCellLine1(tile, tileIndex);
    }
    std::cout << "|" << std::endl;

    // Baris 2: kepemilikan + bidak
    for (int i = 0; i < topCount; i++) {
        int tileIndex = sideLength * 2 + i;
        Tile* tile = board.getTile(tileIndex);
        std::cout << "|" << padRight(getCellLine2(tile, tileIndex), CELL_WIDTH);
    }
    std::cout << "|" << std::endl;

    printHorizontalBorder(topCount);
}

// ==================== Cetak Baris Bawah (index 10..0, kiri ke kanan) ====================

void BoardView::printBottomRow() const{
    Board &board = gameManager.getBoard();
    int sideLength = board.getTileCount() / 4;
    int bottomCount = sideLength + 1;

    // Border atas penuh (menyatu dengan akhir middle)
    printHorizontalBorder(bottomCount);

    // Baris 1
    for (int i = 0; i < bottomCount; i++) {
        int tileIndex = sideLength - i;
        Tile* tile = board.getTile(tileIndex);
        std::cout << "|" << getCellLine1(tile, tileIndex);
    }
    std::cout << "|" << std::endl;

    // Baris 2
    for (int i = 0; i < bottomCount; i++) {
        int tileIndex = sideLength - i;
        Tile* tile = board.getTile(tileIndex);
        std::cout << "|" << padRight(getCellLine2(tile, tileIndex), CELL_WIDTH);
    }
    std::cout << "|" << std::endl;

    printHorizontalBorder(bottomCount);
}

// ==================== Cetak Baris Tengah (kiri + center + kanan) ====================

void BoardView::printMiddleRows()const {
    Board &board = gameManager.getBoard();
    int sideLength = board.getTileCount() / 4;
    int middleCount = sideLength - 1; // 9 baris tengah
    int innerCols = sideLength - 1;
    int innerWidth = (CELL_WIDTH + 1) * innerCols - 1;

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
        int leftIndex  = sideLength * 2 - 1 - row;    // 19, 18, ..., 11
        int rightIndex = sideLength * 3 + 1 + row;    // 31, 32, ..., 39

        Tile* leftTile  = board.getTile(leftIndex);
        Tile* rightTile = board.getTile(rightIndex);

        // 3 slot konten center per row
        int slot0 = row * 3;     // baris 1 sel
        int slot1 = row * 3 + 1; // baris 2 sel
        int slot2 = row * 3 + 2; // baris pemisah (antar sel kiri/kanan)

        // --- Baris 1 sel: kode petak ---
        std::cout << "|" << getCellLine1(leftTile, leftIndex);
        std::cout << "|" << getCenterContent(slot0);
        std::cout << "|" << getCellLine1(rightTile, rightIndex);
        std::cout << "|" << std::endl;

        // --- Baris 2 sel: info kepemilikan + bidak ---
        std::cout << "|" << padRight(getCellLine2(leftTile, leftIndex), CELL_WIDTH);
        std::cout << "|" << getCenterContent(slot1);
        std::cout << "|" << padRight(getCellLine2(rightTile, rightIndex), CELL_WIDTH);
        std::cout << "|" << std::endl;

        // --- Border antar petak: kiri/kanan dash, tengah teks ---
        if (row < middleCount - 1) {
            cellSeparator(getCenterContent(slot2));
        }
    }
}

// ==================== MAIN: drawBoard ====================

void BoardView::drawBoard() const{
    std::cout << std::endl;
    printTopRow();
    printMiddleRows();
    printBottomRow();
    std::cout << std::endl;
}
