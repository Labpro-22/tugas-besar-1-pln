#include "include/models/tile/special-tile/card-tile/ChanceCardTile.hpp"

void ChanceCardTile::onLanded(Player& p, GameManager& gm) {
    CardDeck<ChanceCard*>& cards = gm.getChanceCardDeck();
    ChanceCard* card = cards.drawCard();
    card->takeEffect(p, gm);
}