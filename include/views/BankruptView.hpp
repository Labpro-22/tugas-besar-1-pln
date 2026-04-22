#pragma once
#include "models/player/Player.hpp"
#include <thread>
#include <chrono>
class GameManager;


class BankruptView
{
private:
    GameManager& gameManager;
public:
    BankruptView(GameManager& gm) : gameManager(gm) {}
    void outputPotentialWealth(Player& p, long long debt) const;
    std::pair<std::string, Property*> promptLiquidation(std::vector<Property*> pr, long long debt);
    void outputDebtPaid(long long debt, Player* creditor)const;
    void outputBankruptByBank(Player& p) const;
    void outputBankruptByPlayer(Player& p, Player& creditor) const;

};

