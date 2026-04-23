#pragma once
class GameManager;

class SaveView
{
private:
    GameManager& gameManager;
public:
    SaveView(GameManager& gm) : gameManager(gm) {}
    std::string promptSaveFile() const;
    void outputSaving() const;
    int promptOverwriteSaveFile(std::string path) const;
    void outputSaveStatus(bool success, std::string path) const;
};

