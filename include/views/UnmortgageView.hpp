#pragma once
#include <map>
#include "models/property/Property.hpp"
class GameManager;
class UnmortgageView
{
private:
    GameManager& gameManager;
public:
    UnmortgageView(GameManager& gm) : gameManager(gm) {}
    Property* promptChooseProperty(std::vector<Property*> pr) const;
    void outputUnmortgageStatus(bool success, Property &pr) const;
};


