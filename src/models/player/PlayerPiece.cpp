#include "include/models/player/PlayerPiece.hpp"

PlayerPiece::PlayerPiece(Board* board, int startPosition)
    : board(board), position(startPosition) {}

void PlayerPiece::setPosition(int pos) {
    position = pos;
}

int PlayerPiece::getPosition() const {
    return position;
}

Tile* PlayerPiece::getCurrentTile() const {
    return board->getTile(position);
}

void PlayerPiece::goForward(int tileCount) {
    int boardSize = board->getTileCount();
    int oldPosition = position;
    position = (position + tileCount) % boardSize;
}

void PlayerPiece::goBackward(int tileCount) {
    int boardSize = board->getTileCount();
    position = ((position - tileCount) % boardSize + boardSize) % boardSize;
}
