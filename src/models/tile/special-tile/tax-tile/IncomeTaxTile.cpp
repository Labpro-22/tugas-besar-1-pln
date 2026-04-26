#include "models/tile/special-tile/tax-tile/IncomeTaxTile.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"

void IncomeTaxTile::onLanded(Player&, GameManager& gm) {
    gm.processPayIncomeTax();
}
