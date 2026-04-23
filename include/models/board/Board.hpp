#pragma once

#include <vector>
#include <map>
#include <string>
#include <exception>
#include <cmath>

class Tile;
class Player;
class PlayerPiece;
class PropertyConfig;
class Config;

class Board {
public:
    Board(int tileCount, const Config& config, const std::vector<Player*>& players);

    ~Board();

    int getWidth() const noexcept;

    int getHeight() const noexcept;

    int getTileCount() const noexcept;
    
    Tile* getTile(int position) const; 

    int getTilePosition(const std::string& code) const;

    void setTile(int position, Tile* tile);

    std::vector<Player*> getPlayersAt(int position) const;

    const std::vector<Tile*>& getTiles() const noexcept;

    const std::vector<PlayerPiece*>& getPieces() const noexcept;

    const std::map<std::string, int>& getTilePositions() const noexcept;

    const std::map<std::string, Tile*>& getMapTilesCodeTile() const noexcept;

    const std::map<std::string, std::string>& getMapTilesCodeColor() const noexcept;

    const std::map<std::string, int>& getMapTilesColorCount() const noexcept;

private:
    int width;

    int height;

    std::vector<Tile*> tiles;

    std::vector<PlayerPiece*> pieces;
    
    std::map<std::string, int> tilePositions;

    std::map<std::string, Tile*> mapTilesCodeTile;

    std::map<std::string, std::string> mapTilesCodeColor;
    
    std::map<std::string, int> mapTilesColorCount;

    std::vector<Player*> playersList; 
};