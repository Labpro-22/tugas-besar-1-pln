#include "models/tile/special-tile/card-tile/CommunityChestTile.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"

void CommunityChestTile::onLanded(Player&, GameManager& gm) {
    gm.processUseCommunityChestCard();
}
