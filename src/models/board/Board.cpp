#include "models/board/Board.hpp"
#include "models/tile/PropertyTile.hpp"
#include "models/tile/special-tile/StartTile.hpp"
#include "models/tile/special-tile/JailTile.hpp"
#include "models/tile/special-tile/GoToJailTile.hpp"
#include "models/tile/special-tile/FreeParkingTile.hpp"
#include "models/tile/special-tile/FestiveTile.hpp"
#include "models/tile/special-tile/tax-tile/LuxuryTaxTile.hpp"
#include "models/tile/special-tile/tax-tile/IncomeTaxTile.hpp"
#include "models/tile/special-tile/card-tile/ChanceCardTile.hpp"
#include "models/tile/special-tile/card-tile/CommunityChestTile.hpp"
#include "models/property/StreetProperty.hpp"
#include "models/property/RailroadProperty.hpp"
#include "models/property/UtilityProperty.hpp"
#include "models/player/Player.hpp"
#include "models/player/PlayerPiece.hpp"

Board::Board(int tileCount, const std::vector<int>& tileIDSequence, const Config& config, const std::vector<Player*>& players) : playersList(players) {
    
    this->width = static_cast<int>(std::ceil((tileCount + 4.0) / 4.0));
    this->height = static_cast<int>(std::floor((tileCount + 4.0) / 4.0));

    tiles.resize(tileCount, nullptr);

    int maxRailroad = 0;
    for (const auto& pair : config.railroadRent) {
        if (pair.first > maxRailroad) maxRailroad = pair.first;
    }

    int maxUtility = 0;
    for (const auto& pair : config.utilityRent) {
        if (pair.first > maxUtility) maxUtility = pair.first;
    }

    std::map<int, PropertyConfig> propMap;
    for (const auto& p : config.properties) {
        propMap[p.id] = p;
    }

    for (int i = 0; i < tileCount; ++i) {
        int id = tileIDSequence[i];
        Tile* newTile = nullptr;

        if (propMap.find(id) != propMap.end()) {
            PropertyConfig conf = propMap[id];
            Property* prop = nullptr;

            long long initialFestMul = 1;
            int initialFestDur = 0;

            if (conf.type == "STREET") {
                prop = new StreetProperty(conf.code, conf.name, conf.color, conf.price, conf.mortgageValue, initialFestMul, initialFestDur, conf.housePrice, conf.hotelPrice, conf.rent);
            } 
            else if (conf.type == "RAILROAD") {
                prop = new RailroadProperty(conf.code, conf.name, conf.color, conf.price, conf.mortgageValue, initialFestMul, initialFestDur, config.railroadRent);
            } 
            else if (conf.type == "UTILITY") {
                prop = new UtilityProperty(conf.code, conf.name, conf.color, conf.price, conf.mortgageValue, initialFestMul, initialFestDur, config.utilityRent);
            }

            newTile = new PropertyTile(prop);
            
            mapTilesCodeColor[conf.code] = conf.color;
            mapTilesColorCount[conf.color]++;
        } 
        else {
            switch (id) {
                case 0:   
                    newTile = new StartTile("Petak Mulai", "GO", config.goSalary); 
                    break;
                case 101: 
                    newTile = new CommunityChestTile("Dana Umum", "DNU"); 
                    break;
                case 102: 
                    newTile = new ChanceCardTile("Kesempatan", "KSP"); 
                    break;
                case 103: 
                    newTile = new IncomeTaxTile("Pajak Penghasilan", "PPH", config.incomeFlatTax, config.incomePercentageTax); 
                    break;
                case 104: 
                    newTile = new LuxuryTaxTile("Pajak Barang Mewah", "PBM", config.luxuryFlatTax); 
                    break;
                case 105: 
                    newTile = new FestiveTile("Festival", "FES"); 
                    break;
                case 106: 
                    newTile = new JailTile("Penjara", "PEN"); 
                    break;
                case 107: 
                    newTile = new FreeParkingTile("Bebas Parkir", "BBP"); 
                    break;
                case 108: 
                    newTile = new GoToJailTile("Petak Pergi ke Penjara", "PPJ"); 
                    break;
                default:
                    newTile = nullptr;
                // Still don't know how to set this. Can I just, if not found in propMap I will assign it randomly, tho it depends on the tileIDSequence 
            }
        }

        tiles[i] = newTile;
        
        if (newTile) {
            std::string code = newTile->getCode();
            tilePositions[code] = i;
            mapTilesCodeTile[code] = newTile;
        }
    }

    for (Player* p : playersList) {
        if (p != nullptr) {
            pieces.push_back(&(p->getPiece()));
        }
    }
}

Board::~Board() {
    for (Tile* t : tiles) delete t;
}

int Board::getWidth() const noexcept { 
    return width; 
}

int Board::getHeight() const noexcept { 
    return height; 
}

int Board::getTileCount() const noexcept { 
    return static_cast<int>(tiles.size()); 
}

Tile* Board::getTile(int position) const {
    if (position >= 0 && position < (int)tiles.size()) return tiles[position];
    return nullptr;
}

int Board::getTilePosition(const std::string& code) const {
    auto it = tilePositions.find(code);
    return (it != tilePositions.end()) ? it->second : -1;
}

void Board::setTile(int position, Tile* tile) {
    if (position >= 0 && position < (int)tiles.size()) {
        tiles[position] = tile;
        if (tile) tilePositions[tile->getCode()] = position;
    }
}

std::vector<Player*> Board::getPlayersAt(int position) const {
    std::vector<Player*> result;
    for (Player* p : playersList) {
        if (p && p->getPiece().getPosition() == position) {
            result.push_back(p);
        }
    }
    return result;
}

const std::vector<Tile*>& Board::getTiles() const noexcept { 
    return tiles; 
}

const std::vector<PlayerPiece*>& Board::getPieces() const noexcept { 
    return pieces; 
}

const std::map<std::string, int>& Board::getTilePositions() const noexcept { 
    return tilePositions; 
}

const std::map<std::string, Tile*>& Board::getMapTilesCodeTile() const noexcept { 
    return mapTilesCodeTile; 
}

const std::map<std::string, std::string>& Board::getMapTilesCodeColor() const noexcept { 
    return mapTilesCodeColor; 
}

const std::map<std::string, int>& Board::getMapTilesColorCount() const noexcept { 
    return mapTilesColorCount; 
}