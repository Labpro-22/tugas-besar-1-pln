#include "models/bank/Bank.hpp"

#include <algorithm>
#include <vector>

Bank::Bank(long long initialMoney, Config &config) : initialMoney{initialMoney}
{
    for (PropertyConfig &property : config.properties) {
        if (property.type == "STREET") {
            properties.push_back(
                new StreetProperty{
                    property.code,
                    property.name,
                    property.color,
                    property.price,
                    property.mortgageValue,
                    1,
                    0,
                    property.housePrice,
                    property.hotelPrice,
                    property.rent});
        }
        else if (property.type == "RAILROAD") {
            properties.push_back(
                new RailroadProperty{
                    property.code,
                    property.name,
                    property.color,
                    property.price,
                    property.mortgageValue,
                    1,
                    0,
                    config.railroadRent});
        }
        else if (property.type == "STREET") {
            properties.push_back(
                new UtilityProperty{
                    property.code,
                    property.name,
                    property.color,
                    property.price,
                    property.mortgageValue,
                    1,
                    0,
                    config.utilityRent});
        }
    }
}

Bank::Bank(long long initialMoney, Config &config, std::vector<PropertySaveData> &saveData, std::vector<Player> &players) : initialMoney{initialMoney}
{
    for (PropertyConfig &propertyConfig : config.properties) {
        for (PropertySaveData &propertyData : saveData) {
            if (propertyConfig.code != propertyData.code) continue;
            if (propertyConfig.type == "STREET") {
                StreetProperty *property = new StreetProperty{
                    propertyConfig.code,
                    propertyConfig.name,
                    propertyConfig.color,
                    propertyConfig.price,
                    propertyConfig.mortgageValue,
                    propertyData.festivalMultiplier,
                    propertyData.festivalDuration,
                    propertyConfig.housePrice,
                    propertyConfig.hotelPrice,
                    propertyConfig.rent};
                if (propertyData.hasHotel) {
                    property->buildHouse(4);
                    property->buildHotel();
                }
                else {
                    property->buildHouse(propertyData.houseCount);
                }
                auto player = std::find_if(players.begin(), players.end(), [propertyData](Player &p) {
                    return p.getUsername() == propertyData.owner;
                });
                if (player != players.end()) {
                    property->setOwner(player.base());
                }
                properties.push_back(property);
            }
            else if (propertyConfig.type == "RAILROAD") {
                RailroadProperty *property = new RailroadProperty{
                    propertyConfig.code,
                    propertyConfig.name,
                    propertyConfig.color,
                    propertyConfig.price,
                    propertyConfig.mortgageValue,
                    propertyData.festivalMultiplier,
                    propertyData.festivalDuration,
                    config.railroadRent};
                auto player = std::find_if(players.begin(), players.end(), [propertyData](Player &p) {
                    return p.getUsername() == propertyData.owner;
                });
                if (player != players.end()) {
                    property->setOwner(&*player);
                }
                properties.push_back(property);
            }
            else if (propertyConfig.type == "Utility") {
                UtilityProperty *property = new UtilityProperty{
                    propertyConfig.code,
                    propertyConfig.name,
                    propertyConfig.color,
                    propertyConfig.price,
                    propertyConfig.mortgageValue,
                    propertyData.festivalMultiplier,
                    propertyData.festivalDuration,
                    config.utilityRent};
                auto player = std::find_if(players.begin(), players.end(), [propertyData](Player &p) {
                    return p.getUsername() == propertyData.owner;
                });
                if (player != players.end()) {
                    property->setOwner(player.base());
                }
                properties.push_back(property);
            }
        }
    }
}

Bank::~Bank()
{
    while (!properties.empty()) {
        delete properties.back();
        properties.pop_back();
    }
}

std::vector<Property *> &Bank::getProperties()
{
    return properties;
}

void Bank::giveInitialMoney(std::vector<Player> &players)
{
    giveMoneyToAll(players, initialMoney);
}

void Bank::giveMoney(Player &player, long long amount)
{
    player.receiveMoney(amount);
}

void Bank::giveMoneyToAll(std::vector<Player> &players, long long amount)
{
    for (Player &player : players) {
        player.receiveMoney(amount);
    }
}

void Bank::collectMoney(Player &player, long long amount)
{
    player.payTax(amount);
}

void Bank::collectMoneyFromAll(std::vector<Player> &players, long long amount)
{
    for (Player &player : players) {
        player.payTax(amount);
    }
}