#pragma once

#include "models/card/community-chest-card/CommunityChestCard.hpp"

class PayMoneyToPlayersCard : public CommunityChestCard {
private:
    int amount;

public:
    PayMoneyToPlayersCard(const std::string& message, int amount);
    void takeEffect(Player& p, GameManager& gm) override;
    std::string getCardType() const override;
};