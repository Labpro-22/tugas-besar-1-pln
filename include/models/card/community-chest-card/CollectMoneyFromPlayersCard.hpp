#pragma once

#include "models/card/community-chest-card/CommunityChestCard.hpp"

class CollectMoneyFromPlayersCard : public CommunityChestCard {
private:
    int amount;

public:
    CollectMoneyFromPlayersCard(const std::string& message, int amount);
    void takeEffect(Player& p, GameManager& gm) override;
};