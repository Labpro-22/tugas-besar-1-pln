
#include "views/AuctionView.hpp"
#include "core/GameManager.hpp"
#include "models/player/Player.hpp"
#include <algorithm>
#include <cctype>
#include <sstream>
void AuctionView::outputProperty(Property &pr) const{
    std::cout << "\n=== LELANG PROPERTI ===\n";
    std::cout << "Properti " << pr.getName() << " (" << pr.getCode() << ") akan dilelang!\n";
    std::cout << "Harga beli normal: M" << pr.getPrice() << "\n\n";
    
}
long long AuctionView::promptBidOrPass(Player &p, long long currentBid, const Player* currentLeader, int activeBidderCount) const {
    std::cout << "----------------------------------------\n";
    std::cout << "Giliran bid: " << p.getUsername() << " | Uang: M" << p.getMoney() << "\n";
    std::cout << "Peserta aktif: " << activeBidderCount << "\n";
    if (currentLeader == nullptr) {
        std::cout << "Bid tertinggi: belum ada\n";
    }
    else {
        std::cout << "Bid tertinggi: M" << currentBid << " oleh " << currentLeader->getUsername() << "\n";
    }
    
    while (true) {
        std::cout << "Aksi (PASS / BID <jumlah>, minimal M" << currentBid + 1 << ")\n";
        std::cout << "> ";

        std::string input;
        std::cin >> std::ws;
        std::getline(std::cin, input);

        std::stringstream ss(input);
        std::string command;
        ss >> command;
        std::transform(command.begin(), command.end(), command.begin(),
                       [](unsigned char c) { return static_cast<char>(std::toupper(c)); });

        if (command == "PASS") {
            return 0;
        } 
        else if (command == "BID") {
            long long bid;
            if (ss >> bid) {
                if (bid <= currentBid) {
                    std::cout << "Bid harus lebih besar dari M" << currentBid << ".\n";
                    continue;
                }
                if (bid > p.getMoney()) {
                    std::cout << "Uang " << p.getUsername() << " tidak cukup untuk bid M" << bid << ".\n";
                    continue;
                }
                return bid;
            }
        }

        std::cout << "Aksi Invalid!\n";
    }
}
void AuctionView::outputExcludedPlayer(const Player &p) const{
    std::cout << p.getUsername() << " menolak membeli properti ini, jadi tidak ikut lelang.\n\n";
}

void AuctionView::outputBidAccepted(Player &p, long long bid) const{
    std::cout << "Bid diterima: " << p.getUsername() << " menawar M" << bid << ".\n\n";
}

void AuctionView::outputPass(Player &p, int remainingBidderCount) const{
    std::cout << p.getUsername() << " pass. Peserta aktif tersisa: " << remainingBidderCount << ".\n\n";
}

void AuctionView::outputWinner(Player *p, Property *pr, long long lastBid) const{
    std::cout << "Pemenang: Pemain " << p->getUsername() << "\n";
    std::cout << "Harga akhir: M" << lastBid << "\n";
    std::cout << "Properti " << pr->getName() << " (" << pr->getCode() << ") kini dimiliki Pemain " << p->getUsername() << ".\n";
}
void AuctionView::outputNoBid(Property* pr)const{
    std::cout << pr->getName() <<" tidak dimiliki oleh siapapun!\n\n";
}
