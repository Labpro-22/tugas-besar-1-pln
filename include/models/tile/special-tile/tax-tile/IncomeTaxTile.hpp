#pragma once

#include "TaxTile.hpp"

class IncomeTaxTile: public TaxTile {
    public:
        IncomeTaxTile(const std::string& name, const std::string& code, long long incomeFlatTax, long long incomePercentageTax): Tile(name, code), SpecialTile(code,name), TaxTile(name, code), incomePercentageTax(incomePercentageTax), incomeFlatTax(incomeFlatTax) {}

        void onLanded(Player& p, GameManager& gm) override;

        void onPassBy(Player&, GameManager&) override {}
        
    private:
        long long incomePercentageTax;

        long long incomeFlatTax;
};
