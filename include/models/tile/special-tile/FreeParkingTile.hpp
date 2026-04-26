#pragma once

#include "SpecialTile.hpp"

class FreeParkingTile: public SpecialTile {
    public:
        using SpecialTile::SpecialTile;

        void onLanded(Player&, GameManager&) override {}
    
        void onPassBy(Player&, GameManager&) override {}
};
