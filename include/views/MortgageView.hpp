#pragma once
#include <vector>
#include "models/property/Property.hpp"
#include <map>
#define FORMAT_SPACE_20(a) 20-(a)
#define FORMAT_SPACE_10(a) 10-(a)
class GameManager;

class MortgageView
{
private:
    GameManager& gameManager;
public:
    MortgageView(GameManager& gm) : gameManager(gm) {}
    Property* promptChooseProperty(std::vector<Property*> pr) const;
    void outputMortgageStatus(bool success, Property &pr) const;
    bool sellAllBuildings(std::vector<StreetProperty*> colorGroupProperty);
};
