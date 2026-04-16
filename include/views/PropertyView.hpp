#pragma once
#include "core/GameManager.hpp"

class PropertyView
{
private:
    GameManager& gameManager;
public:
    PropertyView(GameManager& gm) : gameManager(gm) {}
    void outputProperty(Property& pr);
    void outputPlayerProperties(Player& p);
};

