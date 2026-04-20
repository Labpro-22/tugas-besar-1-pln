#include "include/models/card/skill-card/LassoCard.hpp"

LassoCard::LassoCard(const std::string& message) : SkillCard(message) {}

void LassoCard::setTargetPlayer(Player* p) {
    targetPlayer = p;
}

void LassoCard::takeEffect(Player& p, GameManager& gm) {
    if (targetPlayer != nullptr && targetPlayer->getPiece() != nullptr) {
        int currentPos = p.getPiece()->getPosition();
        targetPlayer->getPiece()->setPosition(currentPos);
        std::cout << targetPlayer->getUsername() << " ditarik ke petakmu!\n";
        targetPlayer = nullptr;
    }
}