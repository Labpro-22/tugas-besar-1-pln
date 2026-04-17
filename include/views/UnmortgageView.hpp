#pragma once
#include "core/GameManager.hpp"

class UnmortgageView
{
private:
    GameManager& gameManager;
public:
    UnmortgageView(GameManager& gm) : gameManager(gm) {}
    int promptChooseProperty(std::vector<Property*> pr) const;
    void outputUnmortgageStatus(bool success) const;
};


