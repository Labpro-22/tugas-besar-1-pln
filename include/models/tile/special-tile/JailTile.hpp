#pragma once

#include "SpecialTile.hpp"

class JailTile: public SpecialTile {
    public:
        void onLanded(Player& p, GameManager& gm) override;
    
        void onPassBy(Player& p, GameManager& gm) override;
};