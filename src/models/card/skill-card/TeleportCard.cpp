#include "models/card/skill-card/TeleportCard.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"

TeleportCard::TeleportCard(const std::string& message) : SkillCard(message) {}

void TeleportCard::setTargetPosition(int pos) {
    targetPosition = pos;
}

void TeleportCard::takeEffect(Player& p, GameManager& gm) {
    if (targetPosition >= 0 && targetPosition < 40) {
        p.getPiece().setPosition(targetPosition);
        message = "Anda berpindah ke petak " + std::to_string(targetPosition) + "!";
        gm.getBoard().getTile(targetPosition)->onLanded(p, gm);
        targetPosition = -1; 
    }
}

void TeleportCard::prepareUse(UseSkillCardView& view, GameManager& gm) {
    this->setTargetPosition(view.askForTargetPosition());
}

std::string TeleportCard::getType() const {
    return "TeleportCard"; 
}