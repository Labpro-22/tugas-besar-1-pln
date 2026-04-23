#include "models/tile/special-tile/FestiveTile.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"

void FestiveTile::onLanded(Player& p, GameManager& gm) {
    gm.processStartFestival();
}   