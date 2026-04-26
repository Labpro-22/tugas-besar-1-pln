#pragma once

class GameManager;
class Player;

class TaxView
{
private:
    GameManager& gameManager;
public:
    TaxView(GameManager& gm) : gameManager(gm) {}
    int promptIncomeTax(long long flat, double percentage) const;
    int promptIncomeTax(long long flat, double percentage, Player &player) const;
    void outputLuxuryTax(long long tax)const;
    void outputLuxuryTax(long long tax, Player &player)const;
};
