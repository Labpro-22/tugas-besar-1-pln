#pragma once

class GameManager;

class GameView
{
private:
    GameManager& gameManager;
public:
    GameView(GameManager* gameManager) : gameManager{*gameManager} {}
};