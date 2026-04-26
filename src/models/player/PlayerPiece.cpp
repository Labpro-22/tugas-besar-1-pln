#include "models/player/PlayerPiece.hpp"
#include "models/board/Board.hpp"
#include "models/tile/Tile.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"

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

void PlayerPiece::goForward(int tileCount, Player& player, GameManager& gm) {
    int boardSize = board->getTileCount();
    int oldPos = position;
    int newPos = (oldPos + tileCount) % boardSize;
    if (oldPos + tileCount >= boardSize) {
        Tile* goTile = board->getTile(0);
        if (goTile) {
            goTile->onPassBy(player, gm);
        }
    }
    position = newPos;
}

void PlayerPiece::goBackward(int tileCount) {
    int boardSize = board->getTileCount();
    position = ((position - tileCount) % boardSize + boardSize) % boardSize;
}