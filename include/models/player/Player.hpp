#pragma once

#include "models/player/PlayerException.hpp"
#include "models/property/Property.hpp"
#include "models/property/StreetProperty.hpp"
#include "models/property/RailroadProperty.hpp"
#include "models/property/UtilityProperty.hpp"
#include "models/card/skill-card/SkillCard.hpp"
#include "models/board/Board.hpp"
#include "core/DiceRoller.hpp"

#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <map>

using namespace std;

enum class PlayerState {
  ACTIVE,
  JAILED,
  BANKRUPT
};

class Player
{
public:
    Player(const string& username, long long initialMoney, Board* board);
    Player( const string& username, long long money, PlayerState state, const vector<Property*>& properties, 
            const vector<SkillCard*>& skillCards , int getOutOfJailCardCount, int jailTurns, int position, Board* board);

    PlayerPiece& getPiece();
    long long getMoney() const;
    PlayerState getState() const;
    const string& getUsername() const;
    const vector<Property*>& getProperties() const;
    const vector<SkillCard*>& getSkillCards() const;
    int getStreetPropertyCount() const;
    int getRailroadPropertyCount() const;
    int getUtilityPropertyCount() const;

    void rollDiceAndMove();
    void setDiceAndMove(int value1, int value2);

    long long calculateTotalWealth() const;
    void receiveMoney(long long amount);
    void giveMoney(Player* recipient, long long amount);
    void payRent(Property* pr);
    void payTax(long long amount);

    void bankruptByBank();
    void bankruptByPlayer(Player* creditor);
    bool isBankrupt() const;

    void addProperty(Property* pr);
    void removeProperty(Property* pr);
    void buyProperty(Property* pr);
    void buyProperty(Property* pr, long long bid);
    void sellProperty(Property* pr);
    void mortgageProperty(Property* pr);
    void unmortgageProperty(Property* pr);
    bool isPropertySetComplete(const std::string& color, const Board& board) const;
    void buildOnProperty(StreetProperty* pr);
    void sellBuilding(StreetProperty* pr);
    void addSkillCard(SkillCard* card);
    void useSkillCard(int index);
    void dropSkillCard(int index);
    bool isJailed() const;
    void goToJail();
    void useGetOutOfJailCard();
    void addGetOutOfJailCard();
    void rollToGetOutOfJail();
    void payFineToGetOutOfJail(long long fine);
    void getOutOfJail();
    void onNextTurn();

private:
    string username;
    long long money;
    PlayerState state;
    vector<Property*> properties;
    int streetPropertyCount;
    int railroadPropertyCount;
    int utilityPropertyCount;
    vector<SkillCard*> skillCards;
    PlayerPiece piece;
    int doubleRollCounter;
    int getOutOfJailCardCount;
    int jailTurns;
    pair<int, int> lastRoll;
};

