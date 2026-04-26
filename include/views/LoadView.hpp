#pragma once
class GameManager;

class LoadView
{
private:
    GameManager& gameManager;
public:
    LoadView(GameManager& gm) : gameManager(gm) {}
    void outputSaveNames()const;
    void outputLoading() const;
    void outputLoadStatus(bool success) const;
    std::string promptSaveName();
};

