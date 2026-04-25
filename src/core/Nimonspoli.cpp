#include <exception>
#include <iostream>

#include "core/Nimonspoli.hpp"

void Nimonspoli::run()
{
    try {
        std::cout << "\033[2J\033[1;1H\n";
        GameManager gameManager;
        gameManager.runGame();
    }
    catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}