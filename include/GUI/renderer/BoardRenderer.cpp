#include "renderer/BoardRenderer.hpp"
#include "GuiConfig.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace GuiConfig;

BoardRenderer::BoardRenderer(sf::RenderWindow& w,const sf::Font& f, const sf::Font& b)
    : win(w), font(f), fontBold(b) 
{
    if (!logoTex.loadFromFile("assets/center_pic.png")) {
        std::cout << "[BoardRenderer] GAGAL load texture: assets/center_pic.png\n";
        std::cout << "  Working dir: ";
        system("pwd");   // cek dari mana binary dijalan
    } else {
        std::cout << "[BoardRenderer] Texture loaded: " << logoTex.getSize().x << "x" << logoTex.getSize().y << "\n";
    }
}

sf::Vector2f BoardRenderer::origin(int col, int row) const {
    return { (float)(BOARD_OX + col * TILE_SIZE),
             (float)(BOARD_OY + row * TILE_SIZE) };
}
sf::Vector2f BoardRenderer::originF(float col, float row) const {
    return { (float)BOARD_OX + col * (float)TILE_SIZE,
             (float)BOARD_OY + row * (float)TILE_SIZE };
}

void BoardRenderer::render(const IGuiState& state, const PieceAnimator& anim) {
    drawBackground();
    drawAllTiles(state);
    drawCenter(state);
    drawBorder();
    drawPieces(state, anim);
}

void BoardRenderer::drawBackground() {
    // Dark side panel
    sf::RectangleShape panel({(float)BOARD_PANEL_W,(float)WINDOW_H});
    panel.setFillColor({28,32,44});
    win.draw(panel);

    // Cream board surface
    sf::RectangleShape board({(float)BOARD_PX,(float)BOARD_PX});
    board.setPosition((float)BOARD_OX,(float)BOARD_OY);
    board.setFillColor({245,242,228});
    board.setOutlineColor({60,60,75});
    board.setOutlineThickness(2.f);
    win.draw(board);
}

void BoardRenderer::drawAllTiles(const IGuiState& state) {
    for (const auto& t : state.tiles()) {
        auto [col, row] = PieceAnimator::posToGrid(t.pos);
        drawTile(t, col, row);
    }
}

void BoardRenderer::drawTile(const TileInfo& t, int col, int row) {
    float ts = (float)TILE_SIZE;
    sf::Vector2f o = origin(col, row);

    // Background
    sf::Color bg = style.tileBg(t.tileType);
    sf::RectangleShape rect({ts, ts});
    rect.setPosition(o);
    rect.setFillColor(bg);
    rect.setOutlineColor({120,120,130});
    rect.setOutlineThickness(BORDER_THICK);
    win.draw(rect);

    // Colour band for streets
    if (style.hasColorBand(t.tileType))
        drawColorBand(col, row, style.groupColor(t.colorGroup));

    // Text / symbols
    drawTileText(t, col, row);

    // Ownership & buildings
    drawOwnership(t, col, row);
    drawBuildings(t, col, row);
}

// Placed on the inward-facing edge of each border tile.
void BoardRenderer::drawColorBand(int col, int row, sf::Color c) {
    float ts = (float)TILE_SIZE;
    float bw = (float)COLOR_BAND;
    sf::Vector2f o = origin(col, row);

    sf::Vector2f bSize, bPos;
    if      (row == 10) { bSize={ts,bw}; bPos={o.x,       o.y};       }
    else if (row == 0)  { bSize={ts,bw}; bPos={o.x,       o.y+ts-bw}; }
    else if (col == 0)  { bSize={bw,ts}; bPos={o.x+ts-bw, o.y};       }
    else                { bSize={bw,ts}; bPos={o.x,       o.y};        }
    sf::RectangleShape band(bSize);
    band.setPosition(bPos);

    band.setFillColor(c);
    win.draw(band);
}

void BoardRenderer::drawTileText(const TileInfo& t, int col, int row) {
    float ts  = (float)TILE_SIZE;
    sf::Vector2f o = origin(col, row);
    float midX = o.x + ts/2.f;
    bool corner = (col==0||col==10) && (row==0||row==10);
    sf::Color tc = TileStyle::contrastText(style.tileBg(t.tileType));
    float bandOff = (style.hasColorBand(t.tileType) && row==10)
                     ? (float)COLOR_BAND : 0.f;

    std::string sym = style.symbol(t.tileType);

    if (corner) {
        // Big symbol in centre
        std::string big = sym.empty() ? t.code : sym;
        // Split into two lines if long
        if (big.size() > 6) {
            txt(big.substr(0,6), midX, o.y+14.f, 9, tc, true, true);
            txt(big.substr(6),   midX, o.y+25.f, 9, tc, true, true);
        } else {
            txt(big, midX, o.y + ts*0.30f, 11, tc, true, true);
        }
        // Sub-name
        std::string sub = t.name; if (sub.size()>9) sub=sub.substr(0,9);
        txt(sub, midX, o.y + ts*0.58f, 8, tc, false, true);
        return;
    }

    if (!sym.empty()) {
        // Action / special tile
        txt(sym,    midX, o.y + bandOff + 6.f,  10, {60,60,80}, true, true);
        txt(t.code, midX, o.y + ts - 14.f,       8, tc,         false, true);
    } else {
        // Property tile
        txt(t.code, midX, o.y + bandOff + 2.f,  9, tc, true, true);
        std::string nm = t.name; if (nm.size()>8) nm=nm.substr(0,8);
        txt(nm,     midX, o.y + bandOff + 13.f,  8, tc, false, true);
        if (t.mortgaged) txt("[M]", midX, o.y+bandOff+24.f, 8,{190,50,50},true,true);
    }
}

void BoardRenderer::drawOwnership(const TileInfo& t, int col, int row) {
    if (t.ownerIdx < 0) return;
    sf::Vector2f o = origin(col, row);
    float ts = (float)TILE_SIZE;
    sf::RectangleShape dot({8.f,8.f});
    dot.setFillColor(style.pieceColor(t.ownerIdx));
    dot.setOutlineColor({30,30,30});
    dot.setOutlineThickness(0.5f);
    dot.setPosition(o.x + ts - 10.f, o.y + 2.f);
    win.draw(dot);
}

void BoardRenderer::drawBuildings(const TileInfo& t, int col, int row) {
    if (t.buildings <= 0 || t.tileType != "STREET") return;
    sf::Vector2f o = origin(col, row);
    float ts  = (float)TILE_SIZE;
    bool hotel = (t.buildings == 5);
    int  cnt   = hotel ? 1 : t.buildings;
    float sz   = hotel ? 11.f : 7.f;
    sf::Color col_ = hotel ? sf::Color{200,55,55} : sf::Color{30,155,55};

    float totalW = cnt*(sz+2.f) - 2.f;
    float startX = o.x + ts/2.f - totalW/2.f;
    float startY = o.y + ts - sz - 2.f;

    for (int i = 0; i < cnt; ++i) {
        sf::RectangleShape b({sz,sz});
        b.setFillColor(col_);
        b.setOutlineColor({20,20,20});
        b.setOutlineThickness(0.5f);
        b.setPosition(startX + i*(sz+2.f), startY);
        win.draw(b);
    }
}

void BoardRenderer::drawPieces(const IGuiState& state, const PieceAnimator& anim) {
    const auto& pls = state.players();
    for (int i = 0; i < (int)pls.size(); ++i) {
        if (pls[i].state == "BANKRUPT") continue;

        float col, row;
        anim.gridPos(i, col, row);
        float jmp = anim.jumpOffset(i);

        sf::Vector2f base = originF(col, row);
        float ts = (float)TILE_SIZE;

        // 2×2 sub-grid layout for up to 4 tokens per tile
        float sx = (i%2) * ts*0.42f + ts*0.12f;
        float sy = (i/2) * ts*0.42f + ts*0.28f;
        float px = base.x + sx;
        float py = base.y + sy - jmp;

        float r = (float)PIECE_R;

        // Drop shadow
        sf::CircleShape shadow(r);
        shadow.setFillColor({0,0,0,55});
        shadow.setPosition(px-r+2.f, py-r+3.f);
        win.draw(shadow);

        // Token
        sf::CircleShape piece(r);
        piece.setFillColor(style.pieceColor(i));
        piece.setOutlineColor(style.pieceRim(i));
        piece.setOutlineThickness((float)PIECE_BORDER);
        piece.setPosition(px-r, py-r);
        win.draw(piece);

        // Number label
        sf::Text lbl;
        lbl.setFont(fontBold);
        lbl.setString(std::to_string(i+1));
        lbl.setCharacterSize(9);
        lbl.setFillColor(sf::Color::White);
        auto lb = lbl.getLocalBounds();
        lbl.setPosition(px - lb.width/2.f - lb.left, py - lb.height/2.f - lb.top - 1.f);
        win.draw(lbl);

        // Jail badge
        if (pls[i].state == "JAILED") {
            sf::CircleShape badge(4.f);
            badge.setFillColor({210,40,40});
            badge.setPosition(px+r-4.f, py-r-2.f);
            win.draw(badge);
        }
    }
}

void BoardRenderer::drawCenter(const IGuiState& state) {
    float ts    = (float)TILE_SIZE;
    float inner = ts * 9.f;
    float cx    = (float)BOARD_OX + ts;
    float cy    = (float)BOARD_OY + ts;

    // Background
    sf::RectangleShape bg({inner, inner});
    bg.setPosition(cx, cy);
    bg.setFillColor({240,237,220});
    bg.setOutlineColor({110,105,90});
    bg.setOutlineThickness(1.5f);
    win.draw(bg);

    float midX = cx + inner/2.f;
    float midY = cy + inner/2.f;

    sf::Sprite logo(logoTex);

    float dispW = inner;
    float dispH = inner;
    float scX   = dispW / (float)logoTex.getSize().x;
    float scY   = dispH / (float)logoTex.getSize().y;
    logo.setScale(scX, scY);
    logo.setPosition(midX - dispW / 2.f, midY - dispH / 2.f);

    win.draw(logo);
}

void BoardRenderer::drawBorder() {
    sf::RectangleShape bdr({(float)BOARD_PX,(float)BOARD_PX});
    bdr.setPosition((float)BOARD_OX,(float)BOARD_OY);
    bdr.setFillColor(sf::Color::Transparent);
    bdr.setOutlineColor({35,35,55});
    bdr.setOutlineThickness(3.f);
    win.draw(bdr);
}

void BoardRenderer::txt(const std::string& s, float x, float y,
                         unsigned sz, sf::Color c, bool bold, bool cx) {
    sf::Text t;
    t.setFont(bold ? fontBold : font);
    t.setString(s);
    t.setCharacterSize(sz);
    t.setFillColor(c);
    if (cx) {
        auto b = t.getLocalBounds();
        x -= b.width/2.f + b.left;
    }
    t.setPosition(x, y);
    win.draw(t);
}
