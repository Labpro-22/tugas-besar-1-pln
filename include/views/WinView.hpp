#pragma once
#include "core/GameManager.hpp"
#include <vector>
class WinView
{
private:
    GameManager& gameManager;
public:
    WinView(GameManager& gm) : gameManager(gm) {}
    void outputWinner(std::vector<Player*>);
};

