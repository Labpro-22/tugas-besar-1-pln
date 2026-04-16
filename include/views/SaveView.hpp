#pragma once
#include "../core/GameManager.hpp"

class SaveView
{
private:
    GameManager& gameManager;
public:
    SaveView(GameManager& gm) : gameManager(gm) {}
    void outputSaving();
    int promptOverwriteSaveFile(std::string path);
    void outputSaveStatus(bool success);
};

