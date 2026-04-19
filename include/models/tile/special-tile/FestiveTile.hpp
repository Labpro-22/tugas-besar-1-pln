#pragma once

#include "SpecialTile.hpp"

class FestiveTile: public SpecialTile {
    public:
        void onLanded(Player& p, GameManager& gm) override;
    
};