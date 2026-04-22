#pragma once
#include <thread>
#include <chrono>

class GameManager;

class CardView
{
private:
    GameManager& gameManager;
public:
    CardView(GameManager& gm) : gameManager(gm) {}
    void outputTakeCard(Card &card) const;
};