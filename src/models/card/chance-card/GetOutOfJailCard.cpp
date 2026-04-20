#include "include/models/card/chance/GetOutOfJailCard.hpp"
#include "include/models/Player.hpp"

GetOutOfJailCard::GetOutOfJailCard(const std::string& message) : ChanceCard(message) {}

void GetOutOfJailCard::takeEffect(Player& p, GameManager& gm) {
    p.addGetOutOfJailCard();
    std::cout << "Kartu Bebas Penjara disimpan!\n";
}