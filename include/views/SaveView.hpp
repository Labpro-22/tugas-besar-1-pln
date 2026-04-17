#pragma once
#include "core/GameManager.hpp"

class SaveView
{
private:
    GameManager& gameManager;
public:
    SaveView(GameManager& gm) : gameManager(gm) {}
    void outputSaving() const;
    int promptOverwriteSaveFile(std::string path) const;
    void outputSaveStatus(bool success) const;
};

