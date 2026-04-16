#pragma once

#include <string>

class GameManager;
class Player;

class Tile {
    public:
    Tile(std::string name, std::string code) : name(name), code(code) {}
    
    virtual ~Tile() {}
    
    virtual void onLanded(Player& p, GameManager& gm) = 0;
    
    virtual void onPassBy(Player& p, GameManager& gm) = 0;
    
    std::string getName() const noexcept;

    std::string getCode() const noexcept;

    protected:
        std::string name;

        std::string code;
};