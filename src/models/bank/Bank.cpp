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

Bank::~Bank()
{
    while (!properties.empty()) {
        delete properties.back();
        properties.pop_back();
    }
}

Bank::Bank(Bank&& other) noexcept
    : initialMoney(other.initialMoney), properties(std::move(other.properties))
{
    other.initialMoney = 0;
}

Bank& Bank::operator=(Bank&& other) noexcept
{
    if (this != &other) {
        while (!properties.empty()) {
            delete properties.back();
            properties.pop_back();
        }
        initialMoney = other.initialMoney;
        properties = std::move(other.properties);
        other.initialMoney = 0;
    }
    return *this;
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