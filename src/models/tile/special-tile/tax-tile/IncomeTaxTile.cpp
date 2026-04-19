#include "models/tile/special-tile/tax-tile/IncomeTaxTile.hpp"

void IncomeTaxTile::onLanded(Player& p, GameManager& gm) {
    // Jujur bingung implementasinya gimana, gtw caranya kalau pemain milih bayar flat atau percentage, jadi mungkin uncomment aja yang dibutuhkan
    
    // Jika flat
    // Config config = gm.getConfig();
    // long long incomeFlatTax = config.getIncomeFlatTax();
    // if (p.getMoney() < incomeFlatTax) {
    //     p.bankruptByBank();
    //     return;
    // }
    // p.payTax(incomeFlatTax);
   
    // ===========================================================

    // Jika percentage
    // Config config = gm.getConfig();
    // long long incomePercentageTax = config.getIncomePercentageTax();
    // long long assetsWealth = 0;
    // vector<Property*> properties = p.getProperties();
    // for (int i = 0; i < properties.size(); i++) {
    //     assetsWealth += properties[i]->calculateAssetValue();
    // }
    // long long totalWealth = p.getMoney() + assetsWealth;
    // p.payTax(totalWealth * incomePercentageTax / 100);


}  