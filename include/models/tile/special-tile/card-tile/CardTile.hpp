#pragma once

#include "include/models/tile/special-tile/SpecialTile.hpp"
#include "models/card/community-chest-card/CommunityChestCard.hpp"
class CardTile: virtual public SpecialTile {
    public:
        void onLanded(Player& p, GameManager& gm) override;  
};