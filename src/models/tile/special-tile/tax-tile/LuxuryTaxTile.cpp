#include "models/tile/special-tile/tax-tile/LuxuryTaxTile.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"

void LuxuryTaxTile::onLanded(Player& p, GameManager& gm) {
    p.payTax(luxuryFlatTax);
}  