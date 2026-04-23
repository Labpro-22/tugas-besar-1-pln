#include "models/card/chance-card/ForcedMoveCard.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"

#include <iostream>

ForcedMoveCard::ForcedMoveCard(const std::string& message, int moveOffset) 
    : ChanceCard(message), moveOffset(moveOffset) {}

void ForcedMoveCard::takeEffect(Player& p, GameManager& gm) {
    p.getPiece().goForward(moveOffset);
    int currentPos = p.getPiece().getPosition();
    gm.getBoard().getTile(currentPos)->onLanded(p, gm);
}