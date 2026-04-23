#include "models/tile/special-tile/card-tile/CommunityChestTile.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"

void CommunityChestTile::onLanded(Player& p, GameManager& gm) {
    CardDeck<CommunityChestCard*>& cards = gm.getCommunityChestCardDeck();
    CommunityChestCard* card = cards.drawCard();
    card->takeEffect(p, gm);
}
