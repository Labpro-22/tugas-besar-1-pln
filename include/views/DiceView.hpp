#pragma once
#include "core/GameManager.hpp"

class DiceView
{
private:
    GameManager& gameManager;
public:
    DiceView(GameManager& gm) : gameManager(gm) {}
    void outputRolledDice(int value1, int value2);
    void outputSetDice(int value1, int value2);
};