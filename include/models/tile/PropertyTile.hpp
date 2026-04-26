#pragma once
#include "Tile.hpp"
#include "models/property/Property.hpp"

class Player;
class GameManager;

class PropertyTile: public Tile {
    public:
        PropertyTile(Property* p);

        Property* getProperty() const noexcept;

        void onLanded(Player& p, GameManager& gm) override;

        void onPassBy(Player& p, GameManager& gm) override;
        
    private:
        Property* property;
};