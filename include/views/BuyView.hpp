#pragma once
class GameManager;
#include "views/PropertyView.hpp"
#include "models/property/Property.hpp"
#include "models/player/Player.hpp"

class BuyView
{
private:
    GameManager& gameManager;
public:
    BuyView(GameManager& gm) : gameManager(gm){}
    bool promptBuyProperty(Property& pr);
    bool promptBuyProperty(Property& pr, const Player& buyer);
    void outputBuyStatus(bool success, Property* pr)const;
    void outputBuyStatus(bool success, Property* pr, const Player& buyer)const;
};
