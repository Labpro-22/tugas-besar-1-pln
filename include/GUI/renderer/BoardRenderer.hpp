#pragma once

#include <SFML/Graphics.hpp>
#include "GUI/IGuiState.hpp"
#include "GUI/style/TileStyle.hpp"
#include "GUI/animation/PieceAnimator.hpp"

// TODO : adjust custom size

class BoardRenderer {
public:
    BoardRenderer(sf::RenderWindow& w, const sf::Font& f, const sf::Font& bold);
    void render(const IGuiState& state, const PieceAnimator& anim);

private:
    void drawBackground();
    void drawAllTiles(const IGuiState& state);
    void drawTile(const TileInfo& t, int col, int row);
    void drawColorBand(int col, int row, sf::Color c);
    void drawTileText(const TileInfo& t, int col, int row);
    void drawOwnership(const TileInfo& t, int col, int row);
    void drawBuildings(const TileInfo& t, int col, int row);
    void drawPieces(const IGuiState& state, const PieceAnimator& anim);
    void drawCenter(const IGuiState& state);
    void drawBorder();

    // Helpers
    void txt(const std::string& s, float x, float y, unsigned sz, sf::Color c, bool bold = false, bool cx = false);
    sf::Vector2f origin(int   col, int   row) const;
    sf::Vector2f originF(float col, float row) const;

    sf::RenderWindow& win;
    const sf::Font& font;
    const sf::Font& fontBold;
    TileStyle style;
    sf::Texture logoTex;
};
