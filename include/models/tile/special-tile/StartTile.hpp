#pragma once

#include "SpecialTile.hpp"

class StartTile: public SpecialTile {
    public:
        void onLanded(Player& p, GameManager& gm) override;
    
        void onPassBy(Player& p, GameManager& gm) override;
};