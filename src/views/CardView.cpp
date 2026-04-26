#include "core/GameManager.hpp"
#include "views/CardView.hpp"

void CardView::outputCard(Card& card) const{
    std::cout << "Mengambil kartu";
    for (int i = 0; i < 3; i++) {
        std::cout << ".";
        std::cout.flush(); 
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
    std::cout << "\n";
    std::cout << "+------------------------------+\n";
    std::cout << "| Kartu: " << card.getCardType() << "\n";
    if (!card.getMessage().empty()) {
        std::cout << "| Efek : " << card.getMessage() << "\n";
    }
    std::cout << "+------------------------------+\n";
}

void CardView::outputShielded() const {
    std::cout << "[SHIELD ACTIVE]: Efek ShieldCard melindungi anda. Tidak perlu membayar.\n";
}
