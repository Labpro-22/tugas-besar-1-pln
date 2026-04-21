#pragma once
#include <map>
#include "models/property/Property.hpp"
#define FORMAT_SPACE_20(a) 20-(a)
#define FORMAT_SPACE_10(a) 10-(a)
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


