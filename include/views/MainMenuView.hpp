#pragma once
#include "core/GameManager.hpp"

class MainMenuView
{
private:
    GameManager& gameManager;
public:
    MainMenuView(GameManager& gm) : gameManager(gm) {}
    int promptNewOrLoadGame() const;
};

