#pragma once

#include "TaxTile.hpp"

class LuxuryTaxTile: public TaxTile {
    public:
        void onLanded(Player& p, GameManager& gm) override;  
};