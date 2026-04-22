#include "core/GameManager.hpp"
#include "views/DiceView.hpp"

void DiceView::outputRollDice() const{
    
    std::cout<<"Mengocok dadu";
    for (int i = 0; i < 3; i++) {
        std::cout << ".";
        std::cout.flush(); 
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    int val1 = DiceRoller::getLastRoll().first, val2 = DiceRoller::getLastRoll().second;
    std::cout <<"\n";
    std::cout<< "Hasil: " << val1 << " + " << val2 << " = " <<  val1 + val2 << "\n";
    std::cout << "Memajukan Bidak " << gameManager.getCurrentPlayer().getUsername() << " sebanyak " << val1 + val2 << " petak ";
    for (int i = 0; i < 3; i++) {
        std::cout << ".";
        std::cout.flush(); 
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::cout <<"\n";
}

void DiceView::outputSetDice(int val1, int val2) const  {
    std::cout << "Dadu diatur secara manual.\n";
    std::cout<< "Hasil: " << val1 << " + " << val2 << " = " <<  val1 + val2 << "\n";
    std::cout << "Memajukan Bidak " << gameManager.getCurrentPlayer().getUsername() << " sebanyak " << val1 + val2 << " petak ";
        for (int i = 0; i < 3; i++) {
        std::cout << ".";
        std::cout.flush(); 
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::cout <<"\n";
}