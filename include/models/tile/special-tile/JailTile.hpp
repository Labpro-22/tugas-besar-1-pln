#pragma once

#include "SpecialTile.hpp"

class JailTile: public SpecialTile {
    public:
        using SpecialTile::SpecialTile;

        void onLanded(Player&, GameManager&) override {}
    
        void onPassBy(Player&, GameManager&) override {}
};
