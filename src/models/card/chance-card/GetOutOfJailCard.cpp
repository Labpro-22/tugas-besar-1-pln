#include "models/card/chance-card/GetOutOfJailCard.hpp"

GetOutOfJailCard::GetOutOfJailCard(const std::string& message) : ChanceCard(message) {}

void GetOutOfJailCard::takeEffect(Player& p, GameManager& gm) {
    p.addGetOutOfJailCard();
    message = "Kartu Bebas Penjara disimpan!\n";
}