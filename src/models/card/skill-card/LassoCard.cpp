#include "models/card/skill-card/LassoCard.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"

LassoCard::LassoCard(const std::string& message) : SkillCard(message) {}

void LassoCard::setTargetPlayer(Player* p) {
    targetPlayer = p;
}

void LassoCard::takeEffect(Player& p, GameManager& gm) {
    int currentPos = p.getPiece().getPosition();
    targetPlayer->getPiece().setPosition(currentPos);
    message = targetPlayer->getUsername() + " ditarik ke petakmu!";
    targetPlayer = nullptr;
}

void LassoCard::prepareUse(UseSkillCardView& view, GameManager& gm) {
    this->setTargetPlayer(view.askForTargetPlayer(gm));
}

std::string LassoCard::getType() const {
    return "LassoCard"; 
}