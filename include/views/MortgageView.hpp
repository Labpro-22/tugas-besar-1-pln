#pragma once
#include <vector>
#include "models/property/Property.hpp"
#include <map>
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
