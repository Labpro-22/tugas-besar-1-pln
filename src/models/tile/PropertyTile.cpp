#include "models/tile/PropertyTile.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"

PropertyTile::PropertyTile(Property* p) : Tile(p->getName(), p->getCode()), property(p) {}

Property* PropertyTile::getProperty() const noexcept {
    return property;
}

void PropertyTile::onLanded(Player& p, GameManager& gm) {
    // Print landing message before any buy/rent logic
    gm.processLandingMessage();

    Player* owner = property->getOwner();

    if (owner == nullptr) {
        gm.processBuyProperty();
    }
    else if (owner != &p) {
        gm.processPayRent();
    }
}

void PropertyTile::onPassBy(Player& p, GameManager& gm) {
    // Kosong, tidak melakukan apapun
}