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
    void outputCard(Card &card) const;
};