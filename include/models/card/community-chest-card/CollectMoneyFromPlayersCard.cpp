#include "models/card/community-chest-card/CollectMoneyFromPlayersCard.hpp"
#include <iostream>

CollectMoneyFromPlayersCard::CollectMoneyFromPlayersCard(const std::string& message, int amount) 
    : CommunityChestCard(message), amount(amount) {
}

void CollectMoneyFromPlayersCard::takeEffect(Player& p, GameManager& gm) {
    std::cout << "[Dana Umum] Hari spesial! kamu memperoleh " << amount << " dari setiap pemain.\n";
    
    std::vector<Player*> allPlayers = gm.getPlayers();
    int totalCollected = 0;

    for (Player* other : allPlayers) {
        // Cek agar tidak menarik uang dari diri sendiri
        if (other->getUsername() != p.getUsername()) {
            other->payTax(amount); 
            totalCollected += amount;
        }
    }
    
    p.receiveMoney(totalCollected);
    std::cout << "Total yang kamu kumpulkan: " << totalCollected << ".\n";
}