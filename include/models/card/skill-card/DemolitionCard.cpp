#include "models/card/skill-card/DemolitionCard.hpp"

DemolitionCard::DemolitionCard(const std::string& message) : SkillCard(message) {}

void DemolitionCard::takeEffect(Player& p, GameManager& gm) {
    int targetTileId;
    std::cout << "[Demolition] Masukkan ID petak properti lawan yang ingin dihancurkan: ";
    std::cin >> targetTileId;
    
    Tile* tile = gm.getBoard()->getTile(targetTileId);
    PropertyTile* propTile = dynamic_cast<PropertyTile*>(tile);
    
    if (propTile != nullptr && propTile->getProperty() != nullptr) {
        propTile->getProperty()->resetOwnerAsBank(); 
        std::cout << "BOOM! Properti di petak " << targetTileId << " telah dihancurkan.\n";
    } else {
        std::cout << "ID petak salah atau bukan petak properti.\n";
    }
}