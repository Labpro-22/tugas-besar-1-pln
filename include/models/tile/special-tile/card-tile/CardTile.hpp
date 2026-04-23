#pragma once

#include "models/tile/special-tile/SpecialTile.hpp"
#include "models/card/community-chest-card/CommunityChestCard.hpp"
class CardTile: virtual public SpecialTile {
    public:
        using SpecialTile::SpecialTile;

        void onLanded(Player& p, GameManager& gm) override {}
        
        void onPassBy(Player& p, GameManager& gm) override {} 
};