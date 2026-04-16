#pragma once
#include "../core/GameManager.hpp"

class BoardView
{
private:
    GameManager& gameManager;
public:
    BoardView(GameManager& gm) : gameManager(gm) {}
    void drawBoard();
};


