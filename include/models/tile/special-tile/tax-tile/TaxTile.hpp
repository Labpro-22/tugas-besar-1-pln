#pragma once

#include "models/tile/Tile.hpp"

class TaxTile: virtual public Tile {
    public:
        void onLanded(Player& p, GameManager& gm) override;  
};