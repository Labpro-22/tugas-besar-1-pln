#include "models/tile/special-tile/GoToJailTile.hpp"

void GoToJailTile::onLanded(Player& p, GameManager& gm) {
    p.goToJail();
    p.getPiece().setPosition(gm.getBoard().getTilePosition("PEN"));
}