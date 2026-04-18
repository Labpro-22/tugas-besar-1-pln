#include <exception>
#include <iostream>

#include "core/Nimonspoli.hpp"

void Nimonspoli::run()
{
    try {
        GameManager gameManager;
        gameManager.runGame();
    }
    catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}