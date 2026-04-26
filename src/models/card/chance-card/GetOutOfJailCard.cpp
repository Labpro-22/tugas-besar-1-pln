#include "models/card/chance-card/GetOutOfJailCard.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"

GetOutOfJailCard::GetOutOfJailCard(const std::string& message) : ChanceCard(message) {}

void GetOutOfJailCard::takeEffect(Player& p, GameManager&) {
    p.addGetOutOfJailCard();
}

std::string GetOutOfJailCard::getCardType() const {
    return "GETOUTOFJAILCARD";
}
