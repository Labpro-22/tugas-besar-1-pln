#include "include/models/card/skill-card/TeleportCard.hpp"

TeleportCard::TeleportCard(const std::string& message) : SkillCard(message) {}

void TeleportCard::setTargetPosition(int pos) {
    targetPosition = pos;
}

void TeleportCard::takeEffect(Player& p, GameManager& gm) {
    if (targetPosition >= 0 && targetPosition < 40 && p.getPiece() != nullptr) {
        p.getPiece()->setPosition(targetPosition);
        std::cout << "Berpindah ke petak " << targetPosition << "!\n";
        gm.getBoard()->getTile(targetPosition)->onLanded(&p, &gm);
        targetPosition = -1; 
    }
}