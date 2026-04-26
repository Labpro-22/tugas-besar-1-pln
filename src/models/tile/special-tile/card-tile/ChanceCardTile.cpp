#include "models/tile/special-tile/card-tile/ChanceCardTile.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"

void ChanceCardTile::onLanded(Player&, GameManager& gm) {
    gm.processUseChanceCard();
}
