#pragma once
#include "core/GameManager.hpp"

class BuildView
{
private:
    GameManager& gameManager;
public:
    BuildView(GameManager& gm) : gameManager(gm) {}
    int promptChooseProperty(std::vector<Property*> pr) const;
    void outputBuildStatus(bool success) const;
};

