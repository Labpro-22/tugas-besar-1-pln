#include "models/card/community-chest-card/CollectMoneyFromPlayersCard.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"

#include <iostream>

CollectMoneyFromPlayersCard::CollectMoneyFromPlayersCard(const std::string& message, int amount) 
    : CommunityChestCard(message), amount(amount) {}

void CollectMoneyFromPlayersCard::takeEffect(Player& p, GameManager& gm) {
    std::vector<Player>& allPlayers = gm.getPlayers();

    for (Player& other : allPlayers) {
        if (other.getUsername() != p.getUsername() && !other.isBankrupt()) {
            bool canPay = other.giveMoney(p, amount);
            // If other cannot pay, trigger their liquidation with p as creditor
            if (!canPay) {
                gm.processOtherPlayerLiquidation(other, p);
            }
        }
    }
}

std::string CollectMoneyFromPlayersCard::getCardType() const {
    return "COLLECTMONEYFROMPLAYERSCARD";
}