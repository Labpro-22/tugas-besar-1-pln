#pragma once
class GameManager;
#include "views/PropertyView.hpp"
#include "models/property/Property.hpp"
#define FORMAT_SPACE 30
class BuyView
{
private:
    GameManager& gameManager;
public:
    BuyView(GameManager& gm) : gameManager(gm){}
    bool promptBuyProperty(Property& pr);
    void outputBuyStatus(bool success, Property* pr)const;
};

