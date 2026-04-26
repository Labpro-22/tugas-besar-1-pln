#include "views/JailView.hpp"
#include "core/GameManager.hpp"
void JailView::outputGoToJail()const {
    Player& p = gameManager.getCurrentPlayer();
    if (p.hasEffect("SHIELD")) {
        std::cout << "[SHIELD ACTIVE]: Efek ShieldCard melindungi anda. Tidak perlu pergi ke penjara.\n";
        return;
    }
    std::cout << "Kamu masuk penjara!\n";
    std::cout << "Langsung ke penjara.\n";
    std::cout << "Jangan lewat GO.\n";
    std::cout << "Jangan dapatkan gaji.\n\n";
}
int JailView::promptRollOrBailOrUseCard(){
    Player &p = gameManager.getCurrentPlayer();
    std::cout << "Kamu sedang berada di penjara.\n";
    std::cout << "Apa yang ingin kamu lakukan?\n\n";
    while(true){
        std::cout <<"Pilih aksi:\n";
        std::cout << "1. Lempar dadu (coba keluar dengan mendapatkan angka dobel)\n";
        std::cout << "2. Bayar jaminan (Uang kamu: M" << p.getMoney() << ")\n";
        int cardCount = p.getGetOutOfJailCardCount(); 
        std::cout << "3. Gunakan kartu bebas dari penjara (Jumlah kartu bebas dari penjara yang dimiliki: " << cardCount << " Kartu)\n"; 
        int input ;
        std::cin >> input;
        if(input >= 1 && input <= 3){
            std::cout <<"\n\n";
            if(cardCount == 0 && input == 3){
                std::cout << "Kamu tidak memiliki kartu bebas dari penjara!\n\n";
            }
            else{
                return input;
            }
        }
        else{
            std::cout << "Masukan tidak valid!\n";
        }
    }
}
