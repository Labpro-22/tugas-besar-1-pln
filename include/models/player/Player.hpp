#pragma once

#include "models/player/PlayerPiece.hpp"
#include "models/player/PlayerException.hpp"
#include "models/effect/PlayerEffect.hpp"
#include "models/property/Property.hpp"
#include "models/property/StreetProperty.hpp"
#include "models/property/RailroadProperty.hpp"
#include "models/property/UtilityProperty.hpp"
#include "models/card/skill-card/SkillCard.hpp"
#include "models/board/Board.hpp"
#include "utils/DiceRoller.hpp"
#include "models/player/PlayerPiece.hpp"

#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <map>

class GameManager;

enum class PlayerState {
  ACTIVE,
  JAILED,
  BANKRUPT
};

class Player
{
public:
    Player(const std::string& username, long long initialMoney);
    Player( const std::string& username, long long money, PlayerState state, 
            const std::vector<SkillCard*>& skillCards , int getOutOfJailCardCount, int jailTurns);

    PlayerPiece& getPiece();
    long long getMoney() const;
    PlayerState getState() const;
    const std::string& getUsername() const;
    const std::vector<Property*>& getProperties() const;
    const std::vector<SkillCard*>& getSkillCards() const;
    const std::vector<PlayerEffect>& getEffects() const;
    int getStreetPropertyCount() const;
    int getRailroadPropertyCount() const;
    int getUtilityPropertyCount() const;
    
    bool rollDiceAndMove();
    bool setDiceAndMove(int value1, int value2);

    long long calculateTotalWealth() const;
    void receiveMoney(long long amount);
    void setMoney(long long amount);
    bool giveMoney(Player& recipient, long long amount);
    bool payRent(Property* pr);
    bool payTax(long long amount);

    void bankruptByBank();
    void bankruptByPlayer(Player* creditor);
    bool isBankrupt() const;

    void addProperty(Property* pr);
    void removeProperty(Property* pr);
    bool buyProperty(Property* pr);
    void buyProperty(Property* pr, long long bid);
    void sellProperty(Property* pr);
    void mortgageProperty(Property* pr);
    void unmortgageProperty(Property* pr);
    bool isPropertySetComplete(const std::string& color, const Board& board) const;
    void buildOnProperty(StreetProperty* pr);
    void sellBuilding(StreetProperty* pr);
    void addSkillCard(SkillCard* card);
    void useSkillCard(int index, GameManager& gm);
    void dropSkillCard(int index);
    bool isJailed() const;
    int getGetOutOfJailCardCount()const;
    void goToJail();
    void useGetOutOfJailCard();
    void addGetOutOfJailCard();
    void rollToGetOutOfJail();
    void setDiceToGetOutOfJail(int value1, int value2);
    void payFineToGetOutOfJail(long long fine);
    void getOutOfJail();
    int getJailTurns();
    void addEffect(PlayerEffect effect);
    bool hasEffect(const std::string& name) const;
    int getEffectValue(const std::string& name) const;
    void resetDoubleRollCounter();
    void onNextTurn();

private:
    std::string username;
    long long money;
    PlayerState state;
    std::vector<Property*> properties;
    int streetPropertyCount;
    int railroadPropertyCount;
    int utilityPropertyCount;
    std::vector<SkillCard*> skillCards;
    std::vector<PlayerEffect> effects;
    PlayerPiece piece;
    int doubleRollCounter;
    int getOutOfJailCardCount;
    int jailTurns;
    std::pair<int, int> lastRoll;
};

