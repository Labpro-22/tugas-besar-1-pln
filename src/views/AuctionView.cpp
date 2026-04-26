#include "views/AuctionView.hpp"
#include "core/GameManager.hpp"
#include "models/player/Player.hpp"
#include <sstream>
#include <limits>
#include <algorithm>
#include <cctype>
void AuctionView::outputProperty(Property &pr) const{
    std::cout << "Properti "<< pr.getName() << "(" << pr.getCode() << ")" << " akan dilelang!\n\n";
    
}
long long AuctionView::promptBidOrPass(Player &p) const {
    std::cout << "Giliran: Pemain " << p.getUsername() << " (Uang: M" << p.getMoney() << ")\n";

    while (true) {
        std::cout << "Aksi (PASS / BID <jumlah>)\n";
        std::cout << "> ";

        std::string input;
        std::cin >> std::ws;
        std::getline(std::cin, input);

        std::stringstream ss(input);
        std::string command;
        ss >> command;
        std::transform(command.begin(), command.end(), command.begin(), ::toupper);

        if (command == "PASS") {
            return -1;
        }
        else if (command == "BID") {
            long long bid;
            if (ss >> bid && bid >= 0) {
                if (bid > p.getMoney()) {
                    std::cout << "Bid melebihi uang yang kamu miliki (M" << p.getMoney() << ")!\n";
                    continue;
                }
                return bid;
            }
        }

        std::cout << "Aksi Invalid!\n";
    }
}
void AuctionView::outputWinner(Player *p, Property *pr, long long lastBid) const{
    std::cout << "\nLelang selesai!\n";
    std::cout << "Pemenang: Pemain " << p->getUsername() << "\n";
    std::cout << "Harga akhir: M" << lastBid << "\n\n";
    std::cout << "Properti " << pr->getName() << " (" << pr->getCode() << ") kini dimiliki Pemain " << p->getUsername() << ".\n";
}
void AuctionView::outputNoBid(Property* pr)const{
    std::cout << pr->getName() <<" tidak dimiliki oleh siapapun!\n\n";
}