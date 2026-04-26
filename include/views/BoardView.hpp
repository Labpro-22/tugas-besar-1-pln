#pragma once

class GameManager;

class BoardView
{
private:
    GameManager& gameManager;
    void drawTile(Tile* tile) const;
public:
    const std::string BROWN    = "\033[38;2;139;69;19m";   // Brown
    const std::string RED     = "\033[38;2;255;0;0m";     // Red
    const std::string LIGHT_BLUE = "\033[38;2;173;216;230m"; // Light Blue
    const std::string YELLOW    = "\033[38;2;255;255;0m";   // Yellow
    const std::string PINK      = "\033[38;2;255;105;180m"; // Pink
    const std::string GREEN     = "\033[38;2;0;255;0m";     // Green
    const std::string ORANGE    = "\033[38;2;255;165;0m";   // Orange
    const std::string DARK_BLUE  = "\033[38;2;0;0;139m";     // Dark Blue
    const std::string GREY   = "\033[38;2;128;128;128m"; // Gray
    const std::string DEFAULT = "\033[97m";

    BoardView(GameManager& gm) : gameManager(gm) {}
    void drawBoard() const;
    void outputOnLanded() const;
    void outputOnLanded(Player &p) const;
    void outputOnPassByStart() const;
    std::string getColorCode(const std::string& color)const;
    std::string resetColor()const;
    std::string getColorTag(const std::string& color)const;
    std::string padRight(const std::string& str, int width) const;
    std::string padCenter(const std::string& str, int width) const;
    std::string getOwnershipInfo(Property* pr) const;
    std::string getCellLine1(Tile* tile, int tileIndex) const;
    std::string getPlayersAtPosition(int position)const;
    std::string getCellLine2(Tile* tile, int tileIndex)const;
    void printHorizontalBorder(int count)const;
    std::string getCenterContent(int row) const;
    void printTopRow()const;
    void printBottomRow() const;
    void printMiddleRows()const;
};


