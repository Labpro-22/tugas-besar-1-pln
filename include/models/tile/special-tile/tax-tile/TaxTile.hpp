#pragma once

#include "models/tile/Tile.hpp"

class TaxTile: virtual public Tile {
    public:
        using Tile::Tile;

        void onLanded(Player& p, GameManager& gm) override {}
        
        void onPassBy(Player& p, GameManager& gm) override {}
};