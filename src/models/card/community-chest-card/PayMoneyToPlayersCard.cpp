#include "models/card/community-chest-card/PayMoneyToPlayersCard.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"

#include <iostream>

PayMoneyToPlayersCard::PayMoneyToPlayersCard(const std::string& message, int amount) 
    : CommunityChestCard(message), amount(amount) {}

void PayMoneyToPlayersCard::takeEffect(Player& p, GameManager& gm) {
    std::vector<Player>& allPlayers = gm.getPlayers();
    int expectedTotalPaid = (allPlayers.size() - 1) * amount;
    int moneyNow = p.getMoney() - expectedTotalPaid;
    p.setMoney(moneyNow);
    if (p.getMoney() < 0) {
        gm.processLiquidation();
    }
    if (p.getState() != PlayerState::BANKRUPT) {
        for (Player& other : allPlayers) {
            if (other.getUsername() != p.getUsername()) {
                other.receiveMoney(amount);
            }
        }
    }
}

std::string PayMoneyToPlayersCard::getCardType() const {
    return "PAYMONEYTOPLAYERSCARD";
}
