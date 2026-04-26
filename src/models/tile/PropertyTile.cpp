#include "models/tile/PropertyTile.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"

PropertyTile::PropertyTile(Property* p) : Tile(p->getName(), p->getCode()), property(p) {}

Property* PropertyTile::getProperty() const noexcept {
    return property;
}

void PropertyTile::onLanded(Player& p, GameManager& gm) {
    Player* owner = property->getOwner();

    if (owner == nullptr) {
        gm.processBuyProperty(p);
    }
    else if (owner != &p) {
        gm.processPayRent(p);
    }
}

void PropertyTile::onPassBy(Player&, GameManager&) {
    // Kosong, tidak melakukan apapun
}
