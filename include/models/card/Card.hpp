#pragma once

#include <string>

class Player;
class GameManager;

class Card {
protected:
    std::string message;
public:
    Card(const std::string& message);
    virtual ~Card() = default;
    
    std::string getMessage() const;
    virtual void takeEffect(Player& p, GameManager& gm) = 0;
};