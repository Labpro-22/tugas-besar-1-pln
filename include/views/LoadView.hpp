#pragma once
class GameManager;

class LoadView
{
private:
    GameManager& gameManager;
public:
    LoadView(GameManager& gm) : gameManager(gm) {}
    void outputLoading() const;
    void outputLoadStatus(std::string pathFile, bool fileFound, bool fileNotCorrupted) const;
};

