#pragma once
#include "../core/GameManager.hpp"

class MortgageView
{
private:
    GameManager& gameManager;
public:
    MortgageView(GameManager& gm) : gameManager(gm) {}
    int promptChooseProperty(std::vector<Property*> pr);
    void outputMortgageStatus(bool success);
};
