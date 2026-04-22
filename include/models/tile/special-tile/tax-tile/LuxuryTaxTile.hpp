#pragma once

#include "TaxTile.hpp"

class LuxuryTaxTile: public TaxTile {
    public:
        LuxuryTaxTile(const std::string& name, const std::string& code, long long luxuryFlatTax): Tile(name, code), SpecialTile(code,name), TaxTile(name, code), luxuryFlatTax(luxuryFlatTax) {}

        void onLanded(Player& p, GameManager& gm) override;
        
        void onPassBy(Player& p, GameManager& gm) override {}
        
    private:
        long long luxuryFlatTax;
};