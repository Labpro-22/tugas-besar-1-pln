#include "models/card/community-chest-card/PayMoneyToPlayersCard.hpp"
#include <iostream>

PayMoneyToPlayersCard::PayMoneyToPlayersCard(const std::string& message, int amount) 
    : CommunityChestCard(message), amount(amount) {
}

void PayMoneyToPlayersCard::takeEffect(Player& p, GameManager& gm) {
    std::cout << "[Dana Umum] Sial! Kamu harus membayar denda sebesar " << amount << ".\n";
    p.payTax(amount);
}