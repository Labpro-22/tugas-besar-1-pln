#include <iostream>
#include <stdexcept>

#include "core/Nimonspoli.hpp"

void Nimonspoli::run() const
{
    try {
        GameManager gameManager;
        gameManager.runGame();
    }
    catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}