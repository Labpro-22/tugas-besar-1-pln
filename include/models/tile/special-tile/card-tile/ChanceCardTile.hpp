#pragma once

#include "models/tile/special-tile/card-tile/CardTile.hpp"
#include "models/card/chance-card/ChanceCard.hpp"


class ChanceCardTile: public CardTile {
    public:
        void onLanded(Player& p, GameManager& gm) override;  
};