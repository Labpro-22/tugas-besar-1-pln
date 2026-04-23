#pragma once
class GameManager;

class LoadView
{
private:
    GameManager& gameManager;
public:
    LoadView(GameManager& gm) : gameManager(gm) {}
    bool outputSaveFiles() const;
    std::string promptSaveFile() const;
    void outputLoading() const;
    void outputLoadStatus(std::string pathFile, bool fileFound, bool fileNotCorrupted) const;
};

