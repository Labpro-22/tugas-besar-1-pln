#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <array>

class TileStyle {
public:
    TileStyle();
    sf::Color groupColor(const std::string& colorGroup) const;
    sf::Color tileBg(const std::string& tileType) const;
    sf::Color pieceColor(int playerIdx) const;
    sf::Color pieceRim(int playerIdx) const;
    std::string symbol(const std::string& tileType) const;
    bool  hasColorBand(const std::string& tileType) const;
    static sf::Color contrastText(const sf::Color& bg);
    
private:
    std::map<std::string, sf::Color> group;
    std::map<std::string, sf::Color> bg;
    std::array<sf::Color,4> piece;
    std::array<sf::Color,4> rim;
};
