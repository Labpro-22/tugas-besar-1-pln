#pragma once
#include <map>
#include "models/property/Property.hpp"
class GameManager;

class FestivalView
{
private:
    GameManager& gameManager;
public:
    FestivalView(GameManager& gm) : gameManager(gm) {}
    Property* promptChooseProperty(std::vector<Property*> pr) const;
    void outputFestivalStatus(Property& pr) const;
};

