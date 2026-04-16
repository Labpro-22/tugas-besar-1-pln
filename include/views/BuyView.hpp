#pragma once
#include "../core/GameManager.hpp"

class BuyView
{
private:
    GameManager& gameManager;
public:
    BuyView(GameManager& gm) : gameManager(gm){}
    void outputProperty(Property& pr);
    void outputBuyStatus(bool bought);
};

