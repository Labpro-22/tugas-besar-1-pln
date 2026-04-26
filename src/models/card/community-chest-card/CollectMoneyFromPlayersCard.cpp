#include "models/card/community-chest-card/CollectMoneyFromPlayersCard.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"

#include <iostream>

CollectMoneyFromPlayersCard::CollectMoneyFromPlayersCard(const std::string& message, int amount) 
    : CommunityChestCard(message), amount(amount) {}

void CollectMoneyFromPlayersCard::takeEffect(Player& p, GameManager& gm) {
    std::vector<Player>& allPlayers = gm.getPlayers();

    for (Player& other : allPlayers) {
        //FIX: use HEAD version - checks bankrupt and handles liquidation if other can't pay
        if (other.getUsername() != p.getUsername() && !other.isBankrupt()) {
            bool canPay = other.giveMoney(p, amount);
            if (!canPay) {
                gm.processOtherPlayerLiquidation(other, p);
            }
        }
    }
}

std::string CollectMoneyFromPlayersCard::getCardType() const {
    return "COLLECTMONEYFROMPLAYERSCARD";
}