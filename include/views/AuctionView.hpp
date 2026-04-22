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
    void outputAuctionStatus(Player& p, long long lastBid, Property &pr) const;
};

