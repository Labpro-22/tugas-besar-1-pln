#pragma once

class Board;
class Tile;

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
    
    bool goForward(int tileCount);
    void goBackward(int tileCount);
};

