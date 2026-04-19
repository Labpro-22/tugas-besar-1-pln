#pragma once

#include "SpecialTile.hpp"

class GoToJailTile: public SpecialTile {
    public:
        void onLanded(Player& p, GameManager& gm) override;
    
};