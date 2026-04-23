#include "GuiApp.hpp"
#include "GuiConfig.hpp"
#include "GuiState.hpp"
#include <iostream>

using namespace GuiConfig;

// TODO : add ttf files in assets

GuiApp::GuiApp(std::unique_ptr<IGuiState> state)
    : m_window(sf::VideoMode(WINDOW_W, WINDOW_H),
               "Nimonspoli",
               sf::Style::Titlebar | sf::Style::Close),
      m_state(std::move(state)),
      m_anim(4, 40)
{
    m_window.setFramerateLimit(FPS);

    if (!loadFonts()) {
        std::cerr << "[GUI] Warning: could not load fonts – text may be invisible.\n";
    }

    m_board   = std::make_unique<BoardRenderer>(m_window, m_font, m_bold);
    m_console = std::make_unique<ConsolePanel> (m_window, m_font, m_bold);

    for (const auto& p : m_state->players())
        m_anim.setPos(p.idx, p.pos);
}

bool GuiApp::loadFonts() {
    bool ok = true;
    if (!m_font.loadFromFile(FONT_REG)) {
        if (!m_font.loadFromFile("/usr/share/fonts/truetype/crosextra/Caladea-Regular.ttf")) {
            std::cerr << "[GUI] Could not load regular font.\n";
            ok = false;
        }
    }
    if (!m_bold.loadFromFile(FONT_BOLD)) {
        if (!m_bold.loadFromFile("/usr/share/fonts/truetype/crosextra/Caladea-Bold.ttf")) {
            m_bold = m_font;
        }
    }
    return ok;
}

void GuiApp::run() {
    while (m_window.isOpen()) {
        float dt = m_clock.restart().asSeconds();
        dt = std::min(dt, 0.05f);

        handleEvents();
        update(dt);
        render();
    }
}

void GuiApp::handleEvents() {
    sf::Event e;
    while (m_window.pollEvent(e)) {
        if (e.type == sf::Event::Closed)
            m_window.close();

        m_console->handleEvent(e, *m_state);
    }
}

void GuiApp::update(float dt) {
    m_anim.update(dt);
    m_console->update(dt);

    auto* mock = dynamic_cast<GuiState*>(m_state.get());
    if (mock) {
        mock->update(dt); 
        
        int player, steps;
        if (mock->hasPendingMove(player, steps)) {
            int from = m_state->players()[player].pos;
            m_anim.movePiece(player, from, steps, [this, mock, player, steps, from]() {
                int newPos = ((from - 1 + steps) % 40) + 1;
                mock->setPlayerPos(player, newPos);
            });
        }
    }
}

void GuiApp::render() {
    m_window.clear({22, 26, 38});
    m_board->render(*m_state, m_anim);
    m_console->draw(*m_state);
    m_window.display();
}
