#include "models/player/PlayerPiece.hpp"
#include "models/board/Board.hpp"
#include "models/tile/Tile.hpp"

PlayerPiece::PlayerPiece(int startPosition)
    : board(nullptr), position(startPosition) {}

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

bool PlayerPiece::goForward(int tileCount) {
    int boardSize = board->getTileCount();
    int oldPosition = position;
    position = (position + tileCount) % boardSize;
    return tileCount > 0 && position != 0 && oldPosition + tileCount >= boardSize;
}

void PlayerPiece::goBackward(int tileCount) {
    int boardSize = board->getTileCount();
    position = ((position - tileCount) % boardSize + boardSize) % boardSize;
}
