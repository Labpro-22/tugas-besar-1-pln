#pragma once
#include "core/GameManager.hpp"

class BankruptView
{
private:
    GameManager& gameManager;
public:
    BankruptView(GameManager& gm) : gameManager(gm) {}
    void outputPotentialWealth(Player& p) const;
    void outputBankruptByBank(Player& p) const;
    void outputBankruptByPlayer(Player& p, Player& creditor) const;

};

