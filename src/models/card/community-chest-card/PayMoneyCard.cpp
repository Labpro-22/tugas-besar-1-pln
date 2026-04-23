#include "models/card/community-chest-card/PayMoneyCard.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"

#include <iostream>

PayMoneyCard::PayMoneyCard(const std::string& message, int amount) 
    : CommunityChestCard(message), amount(amount) {}

void PayMoneyCard::takeEffect(Player& p, GameManager& gm) {
    p.payTax(amount);
}