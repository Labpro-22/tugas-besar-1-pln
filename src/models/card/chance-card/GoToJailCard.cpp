#include "models/card/chance-card/GoToJailCard.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"

#include <iostream>

GoToJailCard::GoToJailCard(const std::string& message) : ChanceCard(message) {}

void GoToJailCard::takeEffect(Player& p, GameManager& gm) {
    if (!p.hasEffect("SHIELD")) {
        p.getPiece().setPosition(gm.getBoard().getTilePosition("PEN"));
        p.goToJail();
    }
    return;
}

std::string GoToJailCard::getCardType() const {
    return "GOTOJAILCARD";
}
