#include "models/tile/special-tile/GoToJailTile.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"

void GoToJailTile::onLanded(Player& p, GameManager& gm) {
    p.goToJail();
    p.getPiece().setPosition(gm.getBoard().getTilePosition("PEN"));
}