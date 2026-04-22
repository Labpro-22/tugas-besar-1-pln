#pragma once

#include "SpecialTile.hpp"

class GoToJailTile: public SpecialTile {
    public:
        using SpecialTile::SpecialTile;

        void onLanded(Player& p, GameManager& gm) override;
        
        void onPassBy(Player& p, GameManager& gm) override {}
    
};