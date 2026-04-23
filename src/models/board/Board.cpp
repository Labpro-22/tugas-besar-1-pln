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
#include "utils/config/Config.hpp"

Board::Board(int tileCount, const Config& config, const std::vector<Player*>& players, const std::vector<PropertySaveData>& propertyData) : playersList(players) {
    
    this->width = static_cast<int>(std::ceil((tileCount + 4.0) / 4.0));
    this->height = static_cast<int>(std::floor((tileCount + 4.0) / 4.0));

    tiles.resize(tileCount, nullptr);

    std::map<int, PropertyConfig> propMap;
    std::map<std::string, PropertySaveData> propDataMap;
    for (const auto& p : config.properties) {
        propMap[p.id] = p;
    }
    for (const auto& p : propertyData) {
        propDataMap[p.code] = p;
    }

    std::map<int, ActionTileConfig> actionMap;
    for (const auto& a : config.actionTiles) {
        actionMap[a.id] = a;
    }

    for (int i = 0; i < tileCount; ++i) {
        int id = i + 1;
        Tile* newTile = nullptr;

        if (propMap.find(id) != propMap.end()) {
            PropertyConfig conf = propMap[id];
            Property* prop = nullptr;

            long long initialFestMul = 1;
            int initialFestDur = 0;

            if (conf.type == "STREET") {
                if (propDataMap.count(conf.code)) {
                    PropertySaveData data = propDataMap[conf.code];
                    StreetProperty* property = new StreetProperty(conf.code, conf.name, conf.color, conf.price, conf.mortgageValue, 
                                                                  data.festivalMultiplier, data.festivalDuration, conf.housePrice, conf.hotelPrice, conf.rent);
                    if (data.hasHotel) {
                        property->buildHouse(4);
                        property->buildHotel();
                    }
                    else {
                        property->buildHouse(data.houseCount);
                    }
                    auto player = std::find_if(players.begin(), players.end(), [data](Player &p) {
                        return p.getUsername() == data.owner;
                    });
                    if (player != players.end()) {
                        (*player)->addProperty(property);
                    }
                    prop = property;
                }
                else {
                    prop = new StreetProperty(conf.code, conf.name, conf.color, conf.price, conf.mortgageValue, initialFestMul, initialFestDur, conf.housePrice, conf.hotelPrice, conf.rent);
                }
            } 
            else if (conf.type == "RAILROAD") {
                if (propDataMap.count(conf.code)) {
                    PropertySaveData data = propDataMap[conf.code];
                    RailroadProperty* property = new RailroadProperty(conf.code, conf.name, conf.color, conf.price, conf.mortgageValue, 
                                                                      data.festivalMultiplier, data.festivalDuration, config.railroadRent);
                    auto player = std::find_if(players.begin(), players.end(), [data](Player &p) {
                        return p.getUsername() == data.owner;
                    });
                    if (player != players.end()) {
                        (*player)->addProperty(property);
                    }
                    prop = property;
                }
                else {
                    prop = new RailroadProperty(conf.code, conf.name, conf.color, conf.price, conf.mortgageValue, initialFestMul, initialFestDur, config.railroadRent);
                }
            } 
            else if (conf.type == "UTILITY") {
                if (propDataMap.count(conf.code)) {
                    PropertySaveData data = propDataMap[conf.code];
                    UtilityProperty* property = new UtilityProperty(conf.code, conf.name, conf.color, conf.price, conf.mortgageValue, 
                                                                      data.festivalMultiplier, data.festivalDuration, config.utilityRent);
                    auto player = std::find_if(players.begin(), players.end(), [data](Player &p) {
                        return p.getUsername() == data.owner;
                    });
                    if (player != players.end()) {
                        (*player)->addProperty(property);
                    }
                    prop = property;
                }
                else {
                    prop = new UtilityProperty(conf.code, conf.name, conf.color, conf.price, conf.mortgageValue, initialFestMul, initialFestDur, config.utilityRent);
                }
            }

            if (prop) {
                newTile = new PropertyTile(prop);
                mapTilesCodeColor[conf.code] = conf.color;
                mapTilesColorCount[conf.color]++;
                propertyList.push_back(prop);
            }
        } 
        else if (actionMap.find(id) != actionMap.end()) {
            ActionTileConfig aConf = actionMap[id];
            
            if (aConf.code == "GO") {
                newTile = new StartTile(aConf.name, aConf.code, config.goSalary);
            } 
            else if (aConf.code == "DNU") {
                newTile = new CommunityChestTile(aConf.name, aConf.code);
            } 
            else if (aConf.code == "KSP") {
                newTile = new ChanceCardTile(aConf.name, aConf.code);
            } 
            else if (aConf.code == "PPH") {
                newTile = new IncomeTaxTile(aConf.name, aConf.code, config.incomeFlatTax, config.incomePercentageTax);
            } 
            else if (aConf.code == "PBM") {
                newTile = new LuxuryTaxTile(aConf.name, aConf.code, config.luxuryFlatTax);
            } 
            else if (aConf.code == "FES") {
                newTile = new FestiveTile(aConf.name, aConf.code);
            } 
            else if (aConf.code == "PEN") {
                newTile = new JailTile(aConf.name, aConf.code);
            } 
            else if (aConf.code == "BBP") {
                newTile = new FreeParkingTile(aConf.name, aConf.code);
            } 
            else if (aConf.code == "PPJ") {
                newTile = new GoToJailTile(aConf.name, aConf.code);
            }

            if (newTile) {
                mapTilesCodeColor[aConf.code] = aConf.color;
                mapTilesColorCount[aConf.color]++;
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
    for (Property* p : propertyList) delete p;
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

const std::vector<Property*>& Board::getPropertyList() const noexcept {
    return propertyList;
}