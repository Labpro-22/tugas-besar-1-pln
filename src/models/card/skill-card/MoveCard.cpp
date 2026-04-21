#include "models/card/skill-card/MoveCard.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"
#include <random>

MoveCard::MoveCard(const std::string& message, int amount) 
    : SkillCard(message), amount(amount) {}

void MoveCard::takeEffect(Player& p, GameManager& gm) {
    message = "Maju " + std::to_string(amount) + " langkah.";
    p.getPiece().goForward(amount);
    int currentPos = p.getPiece().getPosition();
    gm.getBoard().getTile(currentPos)->onLanded(p, gm);
}

std::string MoveCard::getType() const {
    return "MoveCard"; 
}