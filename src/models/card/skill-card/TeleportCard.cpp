#include "models/card/skill-card/TeleportCard.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"

TeleportCard::TeleportCard(const std::string& message) : SkillCard(message) {}

void TeleportCard::setTargetPosition(int pos) {
    targetPosition = pos;
}

void TeleportCard::takeEffect(Player& p, GameManager& gm) {
    if (targetPosition >= 0 && targetPosition < gm.getBoard().getTileCount()) {
        p.getPiece().setPosition(targetPosition);
        Tile *targetTile = gm.getBoard().getTile(targetPosition);
        message = "Anda berpindah ke petak " + targetTile->getName() + " [" + targetTile->getCode() + "]!";
        gm.getBoard().getTile(targetPosition)->onLanded(p, gm);
    }
    targetPosition = -1;
}

bool TeleportCard::prepareUse(UseSkillCardView& view, GameManager& gm) {
    (void)gm;
    this->setTargetPosition(view.askForTargetPosition());
    return targetPosition >= 0;
}

std::string TeleportCard::getCardType() const {
    return "TeleportCard"; 
}
