#include "models/player/PlayerPiece.hpp"

PlayerPiece::PlayerPiece(int startPosition)
    : position(startPosition), board(nullptr) {}

void PlayerPiece::setPosition(int pos) {
    position = pos;
}

int PlayerPiece::getPosition() const {
    return position;
}

Tile* PlayerPiece::getCurrentTile() const {
    if (board == nullptr) return nullptr;
    return board->getTile(position);
}

void PlayerPiece::setBoard(Board* board) {
    this->board = board;
}

void PlayerPiece::goForward(int tileCount) {
    int boardSize = board->getTileCount();
    position = (position + tileCount) % boardSize;
}

void PlayerPiece::goBackward(int tileCount) {
    int boardSize = board->getTileCount();
    position = ((position - tileCount) % boardSize + boardSize) % boardSize;
}
