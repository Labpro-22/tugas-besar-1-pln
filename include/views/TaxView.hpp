#pragma once

class GameManager;

class TaxView
{
private:
    GameManager& gameManager;
public:
    TaxView(GameManager& gm) : gameManager(gm) {}
    int promptIncomeTax(long long flat, double percentage) const;
    void outputLuxuryTax(long long tax)const;
};