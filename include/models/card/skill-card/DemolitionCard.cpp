#include "models/card/skill-card/DemolitionCard.hpp"

DemolitionCard::DemolitionCard(const std::string& message) : SkillCard(message) {}

void DemolitionCard::setTargetTileId(int id) {
    targetTileId = id;
}

void DemolitionCard::takeEffect(Player& p, GameManager& gm) {
    if (targetTileId >= 0 && targetTileId < 40) {
        Tile* tile = gm.getBoard()->getTile(targetTileId);
        if (auto* propTile = dynamic_cast<PropertyTile*>(tile)) {
            if (propTile->getProperty() != nullptr) {
                propTile->getProperty()->resetOwnerAsBank();
                std::cout << "Properti di petak " << targetTileId << " telah dihancurkan!\n";
            }
        }
    }
    targetTileId = -1;
}