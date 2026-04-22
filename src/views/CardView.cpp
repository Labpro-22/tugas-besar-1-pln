#include "core/GameManager.hpp"
#include "views/CardView.hpp"

void CardView::outputTakeCard(Card& card) const{
    std::cout <<"Mengambil kartu";
    for (int i = 0; i < 3; i++) {
        std::cout << ".";
        std::cout.flush(); 
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::cout <<"\n";
    std::cout << "Kartu: " << card.getMessage();
}
