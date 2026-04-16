#pragma once

#include "models/board/Board.hpp"
#include "models/tile/Tile.hpp"

class PlayerPiece {
private:
    Board* board;
    int position;

public:
    PlayerPiece(Board* board, int startPosition);

    void setPosition(int pos);
    int getPosition() const;
    Tile* getCurrentTile() const;

    void goForward(int tileCount);
    void goBackward(int tileCount);
};

