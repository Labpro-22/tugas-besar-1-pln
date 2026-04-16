#pragma once
#include "../core/GameManager.hpp"

class CardView
{
private:
    GameManager& gameManager;
public:
    CardView(GameManager& gm) : gameManager(gm) {}
    void outputCard(Card card);
};