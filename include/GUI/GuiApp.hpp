#include <SFML/Graphics.hpp>
#include <memory>
#include "GUI/IGuiState.hpp"
#include "GUI/animation/PieceAnimator.hpp"
#include "GUI/renderer/BoardRenderer.hpp"
#include "GUI/renderer/ConsolePanel.hpp"

class GuiApp {
public:
    explicit GuiApp(std::unique_ptr<IGuiState> state);
    void run();

private:
    void handleEvents();
    void update(float dt);
    void render();
    bool loadFonts();
    sf::RenderWindow m_window;
    sf::Font m_font;
    sf::Font m_bold;
    std::unique_ptr<IGuiState>  m_state;
    PieceAnimator m_anim;
    std::unique_ptr<BoardRenderer> m_board;
    std::unique_ptr<ConsolePanel>  m_console;
    sf::Clock m_clock;
    int m_lastKnownPos[4] = {-1,-1,-1,-1};
};