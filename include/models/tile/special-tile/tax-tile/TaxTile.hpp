#pragma once

#include "models/tile/Tile.hpp"
#include "models/tile/special-tile/SpecialTile.hpp"

class TaxTile: virtual public SpecialTile {
    public:
        using SpecialTile::SpecialTile;

        void onLanded(Player&, GameManager&) override {}
        
        void onPassBy(Player&, GameManager&) override {}
};
