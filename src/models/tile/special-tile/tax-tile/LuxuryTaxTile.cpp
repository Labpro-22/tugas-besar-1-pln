#include "models/tile/special-tile/tax-tile/LuxuryTaxTile.hpp"

void LuxuryTaxTile::onLanded(Player& p, GameManager& gm) {
    const Config& config = gm.getConfig();
    long long luxuryTax = config.getLuxuryTax();
    p.payTax(luxuryTax);
}  