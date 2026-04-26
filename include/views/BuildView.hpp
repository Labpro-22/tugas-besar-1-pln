#pragma once
#include "models/property/Property.hpp"
class GameManager;
#include <map>
class BuildView
{
private:
    GameManager& gameManager;
public:
    BuildView(GameManager& gm) : gameManager(gm) {}
    StreetProperty* promptChooseProperty(std::vector<Property*> pr) const;
    void outputBuildStatus(bool success, StreetProperty* pr) const;
};

