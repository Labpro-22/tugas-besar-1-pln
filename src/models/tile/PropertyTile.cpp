#include "models/tile/PropertyTile.hpp"

PropertyTile::PropertyTile(Property* p) : Tile(p->getName(), p->getCode()), property(p) {}

Property* PropertyTile::getProperty() const noexcept {
    return property;
}

void PropertyTile::onLanded(Player& p, GameManager& gm) {
    Player* owner = property->getOwner();

    if (owner == nullptr) {
        if (p.getMoney() >= property->getPrice()) {
            gm.processBuyProperty();
        }
        else {
            Bank bank = gm.getBank();
            bank.startAuction(*property);
        }
    }
    else if (owner != &p) {
        if (property->isMortgaged()) {
            // "Properti ini sedang digadaikan. Anda bebas biaya sewa!" Idk how to set this thing
        } 
        else {
            p.payRent(property);
        }
    }
    // Idk if the logic correct or not, plis help me :(
}

void PropertyTile::onPassBy(Player& p, GameManager& gm) {
    // Kosong, tidak melakukan apapun
}