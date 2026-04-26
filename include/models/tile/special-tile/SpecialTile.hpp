#pragma once

#include "models/tile/Tile.hpp"

class SpecialTile: virtual public Tile {
    public:
        using Tile::Tile;

        void onLanded(Player&, GameManager&) override {}
    
        void onPassBy(Player&, GameManager&) override {}

};
