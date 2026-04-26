#pragma once

class Board;
class Tile;
class Player;
class GameManager;

class PlayerPiece {
private:
    Board* board;
    int position;

public:
    PlayerPiece(int startPosition);

    void setPosition(int pos);
    int getPosition() const;
    Tile* getCurrentTile() const;
    void setBoard(Board* board);
    
    void goForward(int tileCount);
    void goForward(int tileCount, Player& player, GameManager& gm);
    void goBackward(int tileCount);
};