#include "core/GameManager.hpp"
#include "views/WinView.hpp"

void WinView::outputWinner(Player* winner, std::vector<Player*> remaining)  const{
    if(remaining.size() > 1){
        std::cout << "Permainan selesai! (Batas giliran tercapai)" << "\n\n";
        std::cout << "Rekap pemain:\n\n";
        for(auto player : remaining){
            std::cout << "Pemain " << player->getUsername() << "\n";
            std::cout << "Uang      : M" << player->getMoney() << "\n";
            std::cout << "Properti  : " << player->getProperties().size() << "\n";
            std::cout << "Kartu     : " << player->getSkillCards().size() << "\n\n";
        }
        std::cout <<  "Pemenang : Pemain " << winner->getUsername() << "\n\n";
    }
    else if(remaining.size() == 1){
        std::cout << "Permainan selesai! (Semua pemain kecuali satu bangkrut)\n\n";
        std::cout <<  "Pemain tersisa: \n";
        std::cout << " - Pemain " << winner->getUsername() << "\n\n";
        std::cout << "Pemenang : Pemain " << winner->getUsername() << "\n\n"    ;
    }
    
}