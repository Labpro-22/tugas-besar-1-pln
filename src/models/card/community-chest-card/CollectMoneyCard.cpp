#include "include/models/card/community-chest-card/CollectMoneyCard.hpp"
#include <iostream>

CollectMoneyCard::CollectMoneyCard(const std::string& message, int amount) 
    : CommunityChestCard(message), amount(amount) {}

void CollectMoneyCard::takeEffect(Player& p, GameManager& gm) {
    p.receiveMoney(amount);
    std::cout << "Menerima uang sebesar " << amount << " dari Bank.\n";
}