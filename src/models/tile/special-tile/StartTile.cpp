#include "models/tile/special-tile/StartTile.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"

void StartTile::onLanded(Player& p, GameManager& gm) {
    gm.processGoTile();
    Bank& bank = gm.getBank();
    bank.giveMoney(p, goSalary);
}

void StartTile::onPassBy(Player& p, GameManager& gm) {
    gm.processGoTile();
    Bank& bank = gm.getBank();
    bank.giveMoney(p, goSalary);
}