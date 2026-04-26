#include "core/GameManager.hpp"
#include "GUI/GuiApp.hpp"
#include "GUI/RealGuiState.hpp"
#include <thread>
#include <atomic>
#include <memory>

int main() {
    auto gm = std::make_unique<GameManager>();

    auto rawState = std::make_unique<RealGuiState>(*gm);
    RealGuiState* statePtr = rawState.get();

    gm->setGuiState(statePtr);

    std::atomic<bool> gameRunning{true};
    std::thread gameThread([&]() {
        gm->runGame();
        gameRunning = false;
    });

    GuiApp app(std::move(rawState));
    app.run();

    gm->stopGame();
    if (gameThread.joinable()) gameThread.join();

    return 0;
}