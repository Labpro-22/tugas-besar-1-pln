#include "renderer/Dice.hpp"
#include "GUI/GuiConfig.hpp"
#include <cmath>
#include <vector>
#include <utility>

using namespace GuiConfig;

static const std::vector<std::pair<float,float>> PIP_POS[7] = {
    {},
    {{0.5f,0.5f}},
    {{0.25f,0.25f},{0.75f,0.75f}},
    {{0.25f,0.25f},{0.5f,0.5f},{0.75f,0.75f}},
    {{0.25f,0.25f},{0.75f,0.25f},{0.25f,0.75f},{0.75f,0.75f}},
    {{0.25f,0.25f},{0.75f,0.25f},{0.5f,0.5f},{0.25f,0.75f},{0.75f,0.75f}},
    {{0.25f,0.25f},{0.75f,0.25f},{0.25f,0.5f},{0.75f,0.5f},{0.25f,0.75f},{0.75f,0.75f}},
};

Dice::Dice(const sf::Font& f) : m_font(f) {}

void Dice::update(float dt) {
    m_spinAngle+= dt * 720.f;
    if (m_spinAngle > 360.f) m_spinAngle -= 360.f;
}

void Dice::draw(sf::RenderWindow& win, float x, float y,
                      const DiceInfo& dice) {
    float gap  = 12.f;
    float dieW = (float)DICE_SZ;
    float spin = dice.rolling ? m_spinAngle : 0.f;

    drawDie(win, x,y, dice.d1, dice.rolling, spin);
    drawDie(win, x + dieW+gap,y,dice.d2, dice.rolling, spin * 0.8f);

    // Sum label
    if (!dice.rolling) {
        int sum = dice.d1 + dice.d2;
        sf::Text sum_t;
        sum_t.setFont(m_font);
        sum_t.setString("= " + std::to_string(sum));
        sum_t.setCharacterSize(15);
        sum_t.setFillColor({200,200,220});
        float totalW = dieW * 2.f + gap;
        auto b = sum_t.getLocalBounds();
        sum_t.setPosition(x + totalW/2.f - b.width/2.f - b.left, y + dieW + 5.f);
        win.draw(sum_t);
    }
}

void Dice::drawDie(sf::RenderWindow& win, float x, float y,
                          int face, bool spinning, float spinAngle) {
    float sz  = (float)DICE_SZ;
    float pad = sz * 0.12f;

    float scaleX = 1.f, scaleY = 1.f;
    if (spinning) {
        float r = spinAngle / 360.f;
        scaleX = 0.85f + 0.15f * std::abs(std::cos(r * 3.14159f * 2.f));
        scaleY = 0.85f + 0.15f * std::abs(std::sin(r * 3.14159f * 2.f));
    }

    float dw = sz * scaleX;
    float dh = sz * scaleY;
    float dx = x + (sz - dw) / 2.f;
    float dy = y + (sz - dh) / 2.f;

    // Shadow
    sf::RectangleShape shadow({dw, dh});
    shadow.setPosition(dx+4.f, dy+4.f);
    shadow.setFillColor({0,0,0,80});
    shadow.setOutlineThickness(0.f);
    win.draw(shadow);

    // Die body
    sf::RectangleShape die({dw, dh});
    die.setPosition(dx, dy);
    die.setFillColor({248,246,238});
    die.setOutlineColor({80,78,65});
    die.setOutlineThickness(2.f);
    win.draw(die);

    // Highlight
    sf::RectangleShape hi({dw*0.3f, dh*0.1f});
    hi.setPosition(dx + dw*0.12f, dy + dh*0.10f);
    hi.setFillColor({255,255,255,150});
    win.draw(hi);

    // Dots
    if (!spinning && face >= 1 && face <= 6) {
        float innerSz  = sz - 2.f*pad;
        float dotR     = (float)DICE_DOT_R;
        for (auto& [nx,ny] : PIP_POS[face]) {
            float px = dx + pad + nx * innerSz - dotR;
            float py = dy + pad + ny * innerSz - dotR;
            sf::CircleShape dot(dotR);
            dot.setFillColor({30,28,20});
            dot.setPosition(px, py);
            win.draw(dot);
        }
    } else if (spinning) {
        // ? tex
        sf::Text q; 
        q.setFont(m_font);
        q.setString("?");
        q.setCharacterSize((unsigned) (sz * 0.45f));
        q.setFillColor({100,95,75});
        auto b = q.getLocalBounds();
        q.setPosition(dx + dw/2.f - b.width/2.f - b.left, dy + dh/2.f - b.height/2.f - b.top);
        win.draw(q);
    }
}
