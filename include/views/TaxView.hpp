#pragma once
#include "models/tile/special-tile/tax-tile/IncomeTaxTile.hpp"
#include "models/tile/special-tile/tax-tile/LuxuryTaxTile.hpp"
class GameManager;

class TaxView
{
private:
    GameManager& gameManager;
public:
    TaxView(GameManager gm) : gameManager(gm) {}
    int promptIncomeTax(long long flat, double percentage) const;
    void outputLuxuryTax(long long tax)const;
};