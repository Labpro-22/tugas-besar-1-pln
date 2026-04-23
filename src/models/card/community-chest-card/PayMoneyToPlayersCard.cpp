#include "models/card/community-chest-card/PayMoneyToPlayersCard.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"

#include <iostream>

PayMoneyToPlayersCard::PayMoneyToPlayersCard(const std::string& message, int amount) 
    : CommunityChestCard(message), amount(amount) {}

void PayMoneyToPlayersCard::takeEffect(Player& p, GameManager& gm) {
    std::vector<Player>& allPlayers = gm.getPlayers();
    int totalPaid = 0;

    for (Player& other : allPlayers) {
        if (other.getUsername() != p.getUsername()) {
            p.giveMoney(other, amount);
        }
    }
        
}