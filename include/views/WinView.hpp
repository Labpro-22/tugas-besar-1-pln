#pragma once
#include <vector>
#include "models/player/Player.hpp"
class GameManager;
class WinView
{
private:
    GameManager& gameManager;
public:
    WinView(GameManager& gm) : gameManager(gm) {}
    void outputWinner(Player* winner, std::vector<Player*> pl) const;
};

