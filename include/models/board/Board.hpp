#pragma once

#include <vector>
#include <map>
#include <string>
#include <exception>
#include <cmath>
#include "../Tile/Tile.hpp"

Class PlayerPiece;

class Board {
    public:
        Board(int tileCount, std::vector<int> tileIDSequence, std::vector<propertyConfig> propertiesConfig, std::vector<Player*> players): width(ceil((tileCount + 4)/4)), height(floor((tileCount + 4)/4)) {
            for (int i = 0; i < tileCount; i++) {
                tiles.push_back(new Tile())
            }
        }
        int getWidth() const noexcept;
        int getHeight() const noexcept;
        int getTileCount() const noexcept;
        Tile getTile(int position) const; 
    private:
        int width;
        int height;
        std::vector<Tile*> tiles;
        std::map<std::string, int> tilePositions;
        std::vector<PlayerPiece*> pieces;
};