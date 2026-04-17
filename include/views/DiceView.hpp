#pragma once
#include "core/GameManager.hpp"

class DiceView
{
private:
    GameManager& gameManager;
public:
    DiceView(GameManager& gm) : gameManager(gm) {}
    void outputRollDice() const;
    void outputRolledDice(int value1, int value2) const;
    void outputSetDice(int value1, int value2) const;
};