#pragma once

#include "../Tile.hpp"

class SpecialTile: public Tile {
    public:
        using Tile::Tile;

        void onLanded(Player& p, GameManager& gm) override {}
    
        void onPassBy(Player& p, GameManager& gm) override {}

};