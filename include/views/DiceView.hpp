#pragma once
#include <thread>
#include <chrono>

class GameManager;

class DiceView
{
private:
    GameManager& gameManager;
public:
    DiceView(GameManager& gm) : gameManager(gm) {}
    void outputRollDice(bool moved = true) const;
    void outputSetDice(int value1, int value2, bool moved = true) const;
    void outputSpeedingToJail(int value1, int value2) const;
};
