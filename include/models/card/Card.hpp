#pragma once
#include <string>

class Player;
class GameManager;

class Card {
protected:
    std::string message;
public:
    Card(const std::string& message) : message(message) {}
    virtual ~Card() = default;
    
    std::string getMessage() const { return message; }
    virtual void takeEffect(Player& p, GameManager& gm) = 0;
};