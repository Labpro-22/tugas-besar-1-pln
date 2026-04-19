#pragma once

#include "models/card/community-chest-card/CommunityChestCard.hpp"
#include "models/tile/special-tile/card-tile/CardTile.hpp"

class CommunityChestTile: public CardTile {
    public:
        void onLanded(Player& p, GameManager& gm) override;  
};