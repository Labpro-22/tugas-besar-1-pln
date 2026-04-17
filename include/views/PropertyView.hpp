#pragma once
#include "core/GameManager.hpp"

class PropertyView
{
private:
    GameManager& gameManager;
public:
    PropertyView(GameManager& gm) : gameManager(gm) {}
    void outputRequestTileCode() const;
    void outputPropertyTileNotFound() const;
    void outputProperty(Property& pr) const;
    void outputPlayerProperties(Player& p) const;
};

