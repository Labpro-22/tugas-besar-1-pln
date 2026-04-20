#include "models/card/community-chest-card/PayMoneyToPlayersCard.hpp"
#include <iostream>

PayMoneyCardToPlayersCard::PayMoneyCardToPlayersCard(const std::string& message, int amount) 
    : CommunityChestCard(message), amount(amount) {}

void PayMoneyCard::takeEffect(Player& p, GameManager& gm) {
    p.payTax(amount);
    std::cout << "Membayar denda sebesar " << amount << " ke Bank.\n";
}