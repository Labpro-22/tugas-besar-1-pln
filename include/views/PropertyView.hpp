#pragma once
#include "models/property/Property.hpp"
#include "models/player/Player.hpp"
#define WIDTH 45
#define COLON_WIDTH 30
class GameManager;

class PropertyView
{
private:
    GameManager& gameManager;
public:
    PropertyView(GameManager& gm) : gameManager(gm) {}
    void outputProperty() const;
    void outputPlayerProperties() const;
    void outputRent(Property &pr)const;
};

