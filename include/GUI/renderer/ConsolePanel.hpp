#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "GUI/IGuiState.hpp"
#include "GUI/renderer/Dice.hpp"

class ConsolePanel {
public:
    ConsolePanel(sf::RenderWindow& win, const sf::Font& font, const sf::Font& bold);
    void update(float dt);
    void draw(const IGuiState& state);
    void handleEvent(const sf::Event& e, IGuiState& state);
    const std::string& inputText() const { return input; }

private:
    void drawHeader(const IGuiState& state);
    void drawOutput(const IGuiState& state);
    void drawInputField();
    void drawDice(const IGuiState& state);
    sf::RenderWindow& win;
    const sf::Font& font;
    const sf::Font& bold;
    Dice dice;
    std::string input;
    int scrollOffset;
    float cursorTimer = 0.f; 
    bool  showCursor  = true;
};