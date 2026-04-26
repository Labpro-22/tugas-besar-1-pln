#include "models/card/skill-card/MoveCard.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"
#include <random>

MoveCard::MoveCard(const std::string& message, int amount) 
    : SkillCard(message), amount(amount) {}

int MoveCard::getAmount() const {
    return amount;
}

void MoveCard::takeEffect(Player& p, GameManager& gm) {
    message = "Maju " + std::to_string(amount) + " langkah.";
    bool passedStart = p.getPiece().goForward(amount);
    if (passedStart) {
        gm.getBoard().getTile(0)->onPassBy(p, gm);
    }
    int currentPos = p.getPiece().getPosition();
    gm.getBoard().getTile(currentPos)->onLanded(p, gm);
}

std::string MoveCard::getCardType() const {
    return "MoveCard"; 
}
