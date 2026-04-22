#pragma once

#include "TaxTile.hpp"

class IncomeTaxTile: public TaxTile {
    public:
        IncomeTaxTile(const std::string& name, const std::string& code, long long incomeFlatTax, long long incomePercentageTax): Tile(name, code), TaxTile(name, code), incomeFlatTax(incomeFlatTax), incomePercentageTax(incomePercentageTax) {}

        void onLanded(Player& p, GameManager& gm) override;

        void onPassBy(Player& p, GameManager& gm) override {}
        
    private:
        long long incomePercentageTax;

        long long incomeFlatTax;
};