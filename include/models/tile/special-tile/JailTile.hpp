#pragma once

#include "SpecialTile.hpp"

class JailTile: public SpecialTile {
    public:
        using SpecialTile::SpecialTile;

        void onLanded(Player& p, GameManager& gm) override {}
    
        void onPassBy(Player& p, GameManager& gm) override {}
};