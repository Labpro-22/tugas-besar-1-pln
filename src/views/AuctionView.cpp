
#include "views/AuctionView.hpp"
#include "core/GameManager.hpp"
#include <sstream>
void AuctionView::outputProperty(Property &pr) const{
    std::cout << "Properti "<< pr.getName() << "(" << pr.getCode() << ")" << "akan dilelang!\n\n";
}
long long AuctionView::promptBidOrPass(Player &p) const {
    std::cout << "Giliran: Pemain " << p.getUsername() << "\n";
    
    while (true) {
        std::cout << "Aksi (PASS / BID <jumlah>)\n";
        std::cout << "> ";

        std::string input;
        std::getline(std::cin, input);

        std::stringstream ss(input);
        std::string command;
        ss >> command;

        if (command == "PASS") {
            return 0;
        } 
        else if (command == "BID") {
            long long bid;
            if (ss >> bid) {
                return bid;
            }
        }

        std::cout << "Aksi Invalid!\n";
    }
}
void AuctionView::outputWinner(Player *p, Property *pr, long long lastBid) const{
    std::cout << "Pemenang: Pemain " << p->getUsername() << "\n";
    std::cout << "Harga akhir: M" << lastBid << "\n";
    std::cout << "Properti " << pr->getName() << " (" << pr->getCode() << ") kini dimiliki Pemain " << p->getUsername() << ".\n";
}
void AuctionView::outputNoBid(Property* pr)const{
    std::cout << pr->getName() <<" tidak dimiliki oleh siapapun!\n\n";
}