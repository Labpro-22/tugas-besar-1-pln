#pragma once

#include "TaxTile.hpp"

class IncomeTaxTile: public TaxTile {
    public:
        void onLanded(Player& p, GameManager& gm) override;  
};