#include "models/card/chance-card/ForcedMoveCard.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"
#include "models/tile/Tile.hpp"

#include <iostream>

ForcedMoveCard::ForcedMoveCard(const std::string& message, int moveOffset) 
    : ChanceCard(message), moveOffset(moveOffset) {}

void ForcedMoveCard::takeEffect(Player& p, GameManager& gm) {
    p.getPiece().goForward(moveOffset);
    int currentPos = p.getPiece().getPosition();
    Tile* tile = gm.getBoard().getTile(currentPos);
    if (tile) tile->onLanded(p, gm);
}

std::string ForcedMoveCard::getCardType() const {
    return "FORCEMOVECARD";
}
