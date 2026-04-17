#pragma once
#include "core/GameManager.hpp"

class AuctionView
{
private:
    GameManager& gameManager;
public:
    AuctionView(GameManager& gm) : gameManager(gm) {}
    void outputProperty(Property& pr) const;
    long long promptBidOrPass(Player& p) const;
};

