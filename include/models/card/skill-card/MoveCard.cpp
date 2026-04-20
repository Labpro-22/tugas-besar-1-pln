#include "models/card/skill-card/MoveCard.hpp"
#include <random>

MoveCard::MoveCard(const std::string& message, int amount) 
    : SkillCard(message), amount(amount) {}

void MoveCard::takeEffect(Player& p, GameManager& gm) {
    if (p.getPiece() != nullptr) {
        std::cout << "Maju " << amount << " langkah.\n";
        p.getPiece()->goForward(amount);
        int currentPos = p.getPiece()->getPosition();
        gm.getBoard()->getTile(currentPos)->onLanded(&p, &gm);
    }
}