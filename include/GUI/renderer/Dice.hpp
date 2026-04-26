#pragma once
#include <SFML/Graphics.hpp>
#include "GUI/IGuiState.hpp"

class Dice {
public:
    Dice(const sf::Font& font);
    void update(float dt);
    void draw(sf::RenderWindow& win, float x, float y, const DiceInfo& dice);

private:
    void drawDie(sf::RenderWindow& win, float x, float y, int face, bool spinning, float spinAngle);
    const sf::Font& m_font; 
    float m_spinAngle = 0.f;
};