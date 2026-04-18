#include "models/card/chance/GetOutOfJailCard.hpp"
#include "models/Player.hpp"

GetOutOfJailCard::GetOutOfJailCard(const std::string& message) 
    : ChanceCard(message) {}

void GetOutOfJailCard::takeEffect(Player& p,GameManager& gm) {
    cout << "[Kesempatan] Kamu memperoleh kartu keluar dari penjara!\n";
    p.addGetOutOfJailCard();
}