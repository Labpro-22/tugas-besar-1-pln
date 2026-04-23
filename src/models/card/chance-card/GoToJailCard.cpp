#include "models/card/chance-card/GoToJailCard.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"

#include <iostream>

GoToJailCard::GoToJailCard(const std::string& message) : ChanceCard(message) {}

void GoToJailCard::takeEffect(Player& p, GameManager& gm) {
    p.getPiece().setPosition(gm.getBoard().getTilePosition("PEN"));
    p.goToJail();
}