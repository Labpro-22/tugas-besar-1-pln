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
                                                                  data.festivalMultiplier, data.festivalDuration, 
                                                                  conf.housePrice, conf.hotelPrice, conf.rent,
                                                                  data.hasHotel ? 5 : data.houseCount);

                    auto player = std::find_if(players.begin(), players.end(), [data](Player *p) {
                        return p->getUsername() == data.owner;
                    });
                    if (data.owner != "BANK" && player != players.end()) {
                        (*player)->addProperty(property);
                        property->setOwner(*player);
                        if (data.status == "MORTGAGED") {
                            property->setMortgaged(true);
                        }
        //FIX: resolved merge conflict - setOwner inside owner block
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
                    auto player = std::find_if(players.begin(), players.end(), [data](Player *p) {
                        return p->getUsername() == data.owner;
                    });
                    if (data.owner != "BANK" && player != players.end()) {
                        (*player)->addProperty(property);
                        property->setOwner(*player);
                        if (data.status == "MORTGAGED") {
                            property->setMortgaged(true);
                        }
        //FIX: resolved merge conflict - setOwner inside owner block
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
                    auto player = std::find_if(players.begin(), players.end(), [data](Player *p) {
                        return p->getUsername() == data.owner;
                    });
                    if (data.owner != "BANK" && player != players.end()) {
                        (*player)->addProperty(property);
                        property->setOwner(*player);
                        if (data.status == "MORTGAGED") {
                            property->setMortgaged(true);
                        }
        //FIX: resolved merge conflict - setOwner inside owner block
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
            if (tilePositions.find(code) != tilePositions.end()) {
                std::string uniqueCode = code + "_" + std::to_string(i);
                tilePositions[uniqueCode] = i;
                mapTilesCodeTile[uniqueCode] = newTile;
            } else {
                tilePositions[code] = i;
                mapTilesCodeTile[code] = newTile;
            }
        }
    }

    for (Player* p : playersList) {
        if (p != nullptr) {
            pieces.push_back(&(p->getPiece()));
            p->getPiece().setBoard(this);
        }
    }
}

Board::Board(Board&& other) noexcept
    : width(other.width),
      height(other.height),
      tiles(std::move(other.tiles)),
      pieces(std::move(other.pieces)),
      tilePositions(std::move(other.tilePositions)),
      mapTilesCodeTile(std::move(other.mapTilesCodeTile)),
      mapTilesCodeColor(std::move(other.mapTilesCodeColor)),
      mapTilesColorCount(std::move(other.mapTilesColorCount)),
      playersList(std::move(other.playersList)),
      propertyList(std::move(other.propertyList)) {
    other.width = 0;
    other.height = 0;
    rebindPieces();
}

Board& Board::operator=(Board&& other) noexcept {
    if (this != &other) {
        clear();

        width = other.width;
        height = other.height;
        tiles = std::move(other.tiles);
        pieces = std::move(other.pieces);
        tilePositions = std::move(other.tilePositions);
        mapTilesCodeTile = std::move(other.mapTilesCodeTile);
        mapTilesCodeColor = std::move(other.mapTilesCodeColor);
        mapTilesColorCount = std::move(other.mapTilesColorCount);
        playersList = std::move(other.playersList);
        propertyList = std::move(other.propertyList);

        other.width = 0;
        other.height = 0;
        rebindPieces();
    }

    return *this;
}

Board::~Board() {
    clear();
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

void Board::clear() noexcept {
    for (Tile* t : tiles) {
        delete t;
    }
    for (Property* p : propertyList) {
        delete p;
    }

    tiles.clear();
    pieces.clear();
    tilePositions.clear();
    mapTilesCodeTile.clear();
    mapTilesCodeColor.clear();
    mapTilesColorCount.clear();
    playersList.clear();
    propertyList.clear();
    width = 0;
    height = 0;
}

void Board::rebindPieces() noexcept {
    pieces.clear();
    for (Player* player : playersList) {
        if (player != nullptr) {
            pieces.push_back(&(player->getPiece()));
            player->getPiece().setBoard(this);
        }
    }
}