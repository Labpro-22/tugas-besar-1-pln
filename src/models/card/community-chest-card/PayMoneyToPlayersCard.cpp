#include "models/card/community-chest-card/PayMoneyToPlayersCard.hpp"
#include <iostream>

PayMoneyToPlayersCard::PayMoneyToPlayersCard(const std::string& message, int amount) 
    : CommunityChestCard(message), amount(amount) {}

void PayMoneyToPlayersCard::takeEffect(Player& p, GameManager& gm) {
    p.payTax(amount);
    message = "Anda membayar denda sebesar " + std::to_string(amount) + " ke Bank.";
}