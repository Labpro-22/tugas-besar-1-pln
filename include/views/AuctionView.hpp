#pragma once
#include "models/property/Property.hpp"
class Player;
class GameManager;

class AuctionView
{
private:
    GameManager& gameManager;
public:
    AuctionView(GameManager& gm) : gameManager(gm) {}
    void outputProperty(Property& pr) const;
    long long promptBidOrPass(Player& p, long long currentBid, const Player* currentLeader, int activeBidderCount, bool canPass) const;
    void outputExcludedPlayer(const Player& p) const;
    void outputBidAccepted(Player& p, long long bid) const;
    void outputPass(Player& p, int remainingBidderCount) const;
    void outputWinner(Player* p, Property*pr, long long lastBid) const;
    void outputNoBid(Property *pr)const;
};

