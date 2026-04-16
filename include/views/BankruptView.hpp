#pragma once
#include "core/GameManager.hpp"

class BankruptView
{
private:
    GameManager& gameManager;
public:
    BankruptView(GameManager& gm) : gameManager(gm) {}
    void outputPotentialWealth(Player& p);
    void outputBankruptByBank(Player& p);
    void outputBankruptByPlayer(Player& p, Player& creditor);

};

