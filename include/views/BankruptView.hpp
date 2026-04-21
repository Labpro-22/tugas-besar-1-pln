#pragma once
#include "models/player/Player.hpp"

class GameManager;


class BankruptView
{
private:
    GameManager& gameManager;
public:
    BankruptView(GameManager& gm) : gameManager(gm) {}
    void outputPotentialWealth(Player& p, long long debt) const;
    void outputBankruptByBank(Player& p) const;
    void outputBankruptByPlayer(Player& p, Player& creditor) const;

};

