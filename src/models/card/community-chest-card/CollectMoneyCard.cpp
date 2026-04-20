#include "models/card/community-chest-card/CollectMoneyCard.hpp"
#include <iostream>

CollectMoneyCard::CollectMoneyCard(const std::string& message, int amount) 
    : CommunityChestCard(message), amount(amount) {}

void CollectMoneyCard::takeEffect(Player& p, GameManager& gm) {
    p.receiveMoney(amount);
    message = "Anda menerima uang sebesar " + std::to_string(amount) + "dari Bank";
}