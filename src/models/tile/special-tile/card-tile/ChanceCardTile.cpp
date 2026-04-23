#include "models/tile/special-tile/card-tile/ChanceCardTile.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"

void ChanceCardTile::onLanded(Player& p, GameManager& gm) {
    gm.processUseChanceCard();
}