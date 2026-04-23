#include "models/card/community-chest-card/CollectMoneyFromPlayersCard.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"

#include <iostream>

CollectMoneyFromPlayersCard::CollectMoneyFromPlayersCard(const std::string& message, int amount) 
    : CommunityChestCard(message), amount(amount) {}

void CollectMoneyFromPlayersCard::takeEffect(Player& p, GameManager& gm) {
    std::vector<Player>& allPlayers = gm.getPlayers();
    int totalCollected = 0;

    for (Player& other : allPlayers) {
        if (other.getUsername() != p.getUsername()) {
            other.payTax(amount); 
            totalCollected += amount;
        }
    }
    
    p.receiveMoney(totalCollected);
}