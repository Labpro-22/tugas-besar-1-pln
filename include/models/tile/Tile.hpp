#pragma once

#include <string>

#include "models/bank/Bank.hpp"

class Player;
class GameManager;

class Tile {
    public:
    Tile(const std::string& name, const std::string& code) : name(name), code(code) {}

    virtual ~Tile() {}
    
    virtual void onLanded(Player& p, GameManager& gm) = 0;
    
    virtual void onPassBy(Player& p, GameManager& gm) = 0;
    
    std::string getName() const noexcept {
        return name;
    }

    std::string getCode() const noexcept {
        return code;
    }

    protected:
        std::string name;

        std::string code;
};