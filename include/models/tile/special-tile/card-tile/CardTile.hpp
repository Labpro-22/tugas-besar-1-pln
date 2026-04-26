#pragma once

#include "models/tile/special-tile/SpecialTile.hpp"
#include "models/card/community-chest-card/CommunityChestCard.hpp"
class CardTile: virtual public SpecialTile {
    public:
        using SpecialTile::SpecialTile;

        void onLanded(Player&, GameManager&) override {}
        
        void onPassBy(Player&, GameManager&) override {} 
};
