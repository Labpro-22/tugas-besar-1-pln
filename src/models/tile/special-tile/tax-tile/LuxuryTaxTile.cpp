#include "models/tile/special-tile/tax-tile/LuxuryTaxTile.hpp"

void LuxuryTaxTile::onLanded(Player& p, GameManager& gm) {
    p.payTax(luxuryFlatTax);
}  