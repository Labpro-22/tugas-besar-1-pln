#pragma once
#include "../core/GameManager.hpp"

class LoadView
{
private:
    GameManager& gameManager;
public:
    LoadView(GameManager& gm) : gameManager(gm) {}
    void outputLoading();
    void outputLoadStatus(bool success);
};

