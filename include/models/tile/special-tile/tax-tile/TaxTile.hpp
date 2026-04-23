#pragma once

#include "models/tile/Tile.hpp"
#include "models/tile/special-tile/SpecialTile.hpp"

class TaxTile: virtual public SpecialTile {
    public:
        using SpecialTile::SpecialTile;

        void onLanded(Player& p, GameManager& gm) override {}
        
        void onPassBy(Player& p, GameManager& gm) override {}
};