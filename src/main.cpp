#include "core/Nimonspoli.hpp"
#include "GuiState.hpp"
#include "GuiApp.hpp"

int main(int argc, char *argv[]) {
    if( argc == 1 && argv[0] == "gui"){
        auto state = std::make_unique<GuiState>();
        GuiApp app(std::move(state));
    }
    else {
        Nimonspoli::run();
    }
    return 0;
}