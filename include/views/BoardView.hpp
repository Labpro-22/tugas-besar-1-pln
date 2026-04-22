#pragma once

class GameManager;

class BoardView
{
private:
    GameManager& gameManager;
    void drawTile(bool left, bool top, bool right, bool bottom) const;
public:
    BoardView(GameManager& gm) : gameManager(gm) {}
    void drawBoard() const;
    void outputOnLanded() const;
    void outputOnLanded(Player &p) const;
    void outputOnPassByStart() const;
};


