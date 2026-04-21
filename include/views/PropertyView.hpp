#pragma once
#include "models/property/Property.hpp"
#include "models/player/Player.hpp"
class GameManager;

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

