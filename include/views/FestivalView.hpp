#pragma once
#include "core/GameManager.hpp"

class FestivalView
{
private:
    GameManager& gameManager;
public:
    FestivalView(GameManager& gm) : gameManager(gm) {}
    int promptChooseProperty(std::vector<Property*> pr);
    void outputFestivalStatus(Property& pr);
};

