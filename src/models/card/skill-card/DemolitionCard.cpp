#include "models/card/skill-card/DemolitionCard.hpp"
#include "models/player/Player.hpp"
#include "models/tile/PropertyTile.hpp"
#include "core/GameManager.hpp"

DemolitionCard::DemolitionCard(const std::string& message) : SkillCard(message) {}

void DemolitionCard::setTargetTileId(int id) {
    targetTileId = id;
}

void DemolitionCard::takeEffect(Player& p, GameManager& gm) {
    (void)p;
    if (targetTileId >= 0 && targetTileId < gm.getBoard().getTileCount()) {
        Tile* tile = gm.getBoard().getTile(targetTileId);
        if (auto* propTile = dynamic_cast<PropertyTile*>(tile)) {
            Property *property = propTile->getProperty();
            if (property != nullptr) {
                if (Player *owner = property->getOwner(); owner != nullptr) {
                    owner->removeProperty(property);
                }
                property->resetToBank();
                message = "Properti " + property->getName() + " [" + property->getCode() + "] telah dihancurkan dan kembali ke bank!";
            }
        }
    }
    targetTileId = -1;
}

bool DemolitionCard::prepareUse(UseSkillCardView& view, GameManager& gm) {
    this->setTargetTileId(view.askForDemolitionTileId());
    return targetTileId >= 0;
}

std::string DemolitionCard::getCardType() const {
    return "DemolitionCard";
}
