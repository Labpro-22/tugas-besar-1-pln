#include "models/card/community-chest-card/CollectMoneyCard.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"

#include <iostream>

CollectMoneyCard::CollectMoneyCard(const std::string& message, int amount) 
    : CommunityChestCard(message), amount(amount) {}

void CollectMoneyCard::takeEffect(Player& p, GameManager& gm) {
    p.receiveMoney(amount);
}

std::string CollectMoneyCard::getCardType() const {
    return "COLLECTMONEYCARD";
}
