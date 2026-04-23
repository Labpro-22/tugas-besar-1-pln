#include <SFML/Graphics.hpp>
#include <memory>
#include "IGuiState.hpp"

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
    sf::Clock m_clock;
};
