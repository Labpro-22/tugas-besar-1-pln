#include "models/tile/special-tile/StartTile.hpp"

void StartTile::onLanded(Player& p, GameManager& gm) {
    Bank& bank = gm.getBank();
    bank.giveMoney(p, gm.getConfig().goSalary);
}
    
void StartTile::onPassBy(Player& p, GameManager& gm) {
    Bank& bank = gm.getBank();
    bank.giveMoney(p, gm.getConfig().goSalary);
}