#include "GUI/GuiApp.hpp"
#include "GUI/GuiConfig.hpp"
#include "GUI/RealGuiState.hpp"
#include <iostream>

using namespace GuiConfig;


GuiApp::GuiApp(std::unique_ptr<IGuiState> state)
    : m_state(std::move(state))
    , m_anim(4, 40)
{
    if (!loadFonts()) {
        std::cerr << "[GuiApp] Font load failed — using default\n";
    }

    m_window.create(
        sf::VideoMode(WINDOW_W, WINDOW_H),
        "Nimonspoli",
        sf::Style::Titlebar | sf::Style::Close
    );
    m_window.setFramerateLimit(FPS);

    m_board   = std::make_unique<BoardRenderer>(m_window, m_font, m_bold);
    m_console = std::make_unique<ConsolePanel>(m_window, m_font, m_bold);

    for (int i = 0; i < 4; ++i) m_lastKnownPos[i] = -1;
}

bool GuiApp::loadFonts() {
    const char* regPaths[] = {
        GuiConfig::FONT_REG,
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
        "/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf",
        nullptr
    };
    const char* boldPaths[] = {
        GuiConfig::FONT_BOLD,
        "/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf",
        "/usr/share/fonts/truetype/ubuntu/Ubuntu-B.ttf",
        nullptr
    };

    bool ok = false;
    for (int i = 0; regPaths[i]; ++i) {
        if (m_font.loadFromFile(regPaths[i])) { ok = true; break; }
    }
    for (int i = 0; boldPaths[i]; ++i) {
        if (m_bold.loadFromFile(boldPaths[i])) break;
    }
    if (!m_bold.loadFromFile(GuiConfig::FONT_BOLD)) m_bold = m_font;
    return ok;
}

void GuiApp::run() {
    while (m_window.isOpen()) {
        float dt = m_clock.restart().asSeconds();
        if (dt > 0.1f) dt = 0.1f;
        handleEvents();
        update(dt);
        render();
    }
}


void GuiApp::handleEvents() {
    sf::Event e;
    while (m_window.pollEvent(e)) {
        if (e.type == sf::Event::Closed) {
            m_window.close();
            return;
        }
        m_console->handleEvent(e, *m_state);
    }
}


void GuiApp::update(float dt) {
    if (auto* real = dynamic_cast<RealGuiState*>(m_state.get())) {
        real->updateDiceTimer(dt);

        int pidx = 0, fromPos = 1, steps = 0;
        if (real->takePendingMove(pidx, fromPos, steps)) {
            if (steps <= 0) {
                m_anim.setPos(pidx, fromPos);
                m_lastKnownPos[pidx] = fromPos;
            } else {
                m_anim.movePiece(pidx, fromPos, steps, [this, pidx]() {
                    const auto& pls = m_state->players();
                    if (pidx < (int)pls.size())
                        m_lastKnownPos[pidx] = pls[pidx].pos;
                });
            }
        }
    }

    const auto& pls = m_state->players();
    for (int i = 0; i < (int)pls.size(); ++i) {
        if (m_anim.isAnimating(i)) continue;           // animation in flight — skip
        int newPos = pls[i].pos;
        if (newPos < 1) continue;                       // player data not ready
        if (m_lastKnownPos[i] != newPos) {
            m_anim.setPos(i, newPos);
            m_lastKnownPos[i] = newPos;
        }
    }

    m_anim.update(dt);
    m_console->update(dt);
}


void GuiApp::render() {
    m_window.clear({28, 32, 44, 255});
    m_board->render(*m_state, m_anim);
    m_console->draw(*m_state);
    m_window.display();
}