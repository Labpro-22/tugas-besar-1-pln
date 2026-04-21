#pragma once
class GameManager;
class LogView
{
private:
    GameManager& gameManager;
public:
    LogView(GameManager& gm) : gameManager(gm){};
    void printLogView() const;
    void printLogView(int logAmount) const;
};

