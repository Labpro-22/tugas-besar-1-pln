#pragma once
#include "models/property/Property.hpp"
#include "models/player/Player.hpp"
class GameManager;

class AuctionView
{
private:
    GameManager& gameManager;
public:
    AuctionView(GameManager& gm) : gameManager(gm) {}
    void outputProperty(Property& pr) const;
    long long promptBidOrPass(Player& p) const;
    void outputWinner(Player* p, Property*pr, long long lastBid) const;
    void outputNoBid(Property *pr)const;
};

