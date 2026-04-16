#pragma once
#include "../core/GameManager.hpp"

class UnmortgageView
{
private:
    GameManager& gameManager;
public:
    UnmortgageView(GameManager& gm) : gameManager(gm) {}
    int promptChooseProperty(std::vector<Property*> pr);
    void outputUnmortgageStatus(bool success);
};


