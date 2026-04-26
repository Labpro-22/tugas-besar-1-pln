#include "models/card/skill-card/LassoCard.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"

LassoCard::LassoCard(const std::string& message) : SkillCard(message) {}

void LassoCard::setTargetPlayer(Player* p) {
    targetPlayer = p;
}

void LassoCard::takeEffect(Player& p, GameManager& gm) {
    if (targetPlayer == nullptr) {
        message = "Kartu batal digunakan.";
        return;
    }
    int currentPos = p.getPiece().getPosition();
    targetPlayer->getPiece().setPosition(currentPos);
    Tile *currentTile = p.getPiece().getCurrentTile();
    message = targetPlayer->getUsername() + " ditarik ke petak " + currentTile->getName() + " [" + currentTile->getCode() + "]!";
    currentTile->onLanded(*targetPlayer, gm);
    targetPlayer = nullptr;
}

bool LassoCard::prepareUse(UseSkillCardView& view, GameManager& gm) {
    this->setTargetPlayer(view.askForTargetPlayer(gm));
    return targetPlayer != nullptr;
}

std::string LassoCard::getCardType() const {
    return "LassoCard"; 
}
