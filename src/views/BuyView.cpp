#include "core/GameManager.hpp"
#include "views/BuyView.hpp"
#include <thread>
#include <chrono>

bool BuyView::promptBuyProperty(Property& pr){
    return promptBuyProperty(pr, gameManager.getCurrentPlayer());
}

bool BuyView::promptBuyProperty(Property& pr, const Player& buyer){
    Property *p = &pr;
    if (auto sp = dynamic_cast<StreetProperty*>(p)) {
        auto centerText = [&](const std::string& text) {
            int padding = WIDTH - text.length();
            int left = padding / 2;
            int right = padding - left;
            return std::string(left, ' ') + text + std::string(right, ' ');
        };
        std::cout << "+" << std::string(WIDTH, '=') << "+\n";
        std::cout << "|" <<  centerText("AKTA KEPEMILIKAN") << "|\n";
        std::cout << "|" << centerText("[" + sp->getColor() +"] " + sp->getName() + " (" + sp->getCode() + ")") <<"|\n";
        std::cout << "+" << std::string(WIDTH, '=') << "+\n";
        std::cout << "|" << std::left << std::setw(COLON_WIDTH) << " Harga Beli" << std::left << std::setw(WIDTH-COLON_WIDTH) << ": M" + std::to_string( sp->getPrice())<<"|\n";
        std::cout << "|" << std::left << std::setw(COLON_WIDTH) << " Nilai Gadai" << std::left << std::setw(WIDTH-COLON_WIDTH) << ": M" + std::to_string( sp->getMortgageValue())<<"|\n";
        std::cout << "+" <<std::string(WIDTH, '-') << "+\n";
        std::array<long long, 6> rent= sp->getRentPrice();
        for(int i=0; i < 6; i++){
            if(i == 0){
                std::cout <<"|" << std::left << std::setw(COLON_WIDTH) << " Sewa (unimproved)" << std::left << std::setw(WIDTH-COLON_WIDTH) << (": M" + std::to_string(rent[0])) << "|\n";
            }
            // i==5 is hotel, must check before i>0&&i<5
            else if(i == 5){
                std::cout<< "|" <<std::left << std::setw(COLON_WIDTH) << " Sewa (hotel)" << std::left << std::setw(WIDTH-COLON_WIDTH) << (": M" + std::to_string(rent[5])) << "|\n";
            }
            else if(i > 0 && i < 5){
                std::cout << "|" << std::left << std::setw(COLON_WIDTH) << (" Sewa (" + std::to_string(i) + " rumah)") << std::left << std::setw(WIDTH-COLON_WIDTH) << (": M" + std::to_string(rent[i])) << "|\n";
            }
        }
        std::cout << "+" << std::string(WIDTH,'-') << "+\n";
        std::cout << "|" << std::left << std::setw(COLON_WIDTH) << " Harga Rumah" << std::left << std::setw(WIDTH-COLON_WIDTH) << ( ": M" + std::to_string(sp->getHousePrice()))<<"|\n";
        std::cout << "|" <<std::left << std::setw(COLON_WIDTH) << " Harga Hotel" << std::left << std::setw(WIDTH-COLON_WIDTH) << (": M" + std::to_string(sp->getHotelPrice()))<<"|\n";
        std::cout << "+" << std::string(WIDTH, '=') << "+\n";
        std::cout << "Uang Pemain " << buyer.getUsername() << " saat ini: M" << buyer.getMoney() << "\n";
        std::string yayOrNay;
        std::cout << "Apakah kamu ingin membeli properti ini seharga M" << sp->getPrice() << "? (y/n): ";
        while(true){
            std::cin >> yayOrNay;
            if(yayOrNay == "y"){
                return true;
            }
            if(yayOrNay == "n"){
                return false;
            }
            std::cout << "\nInput tidak valid! Apakah kamu ingin membeli properti ini seharga M" << sp->getPrice() << "? (y/n): ";
        }
    }
    else {
        if(p->getPropertyType() == "UTILITY"){
            std::cout<<"Belum ada yang menginjaknya duluan, " << p->getName() << " kini menjadi milik Pemain " << buyer.getUsername() << "!\n\n";
        }
        else if(p->getPropertyType() == "RAILROAD"){
            std::cout<<"Belum ada yang menginjaknya duluan, stasiun ini kini menjadi milik Pemain " << buyer.getUsername() << "!\n\n";
        }
        return true;
    }
}

void BuyView::outputBuyStatus(bool success, Property* pr)const{
    outputBuyStatus(success, pr, gameManager.getCurrentPlayer());
}

void BuyView::outputBuyStatus(bool success, Property* pr, const Player& buyer)const{
    if(success){
        std::cout << pr->getName() <<  " kini dimiliki Pemain " << buyer.getUsername() << "!\nUang tersisa: M" << buyer.getMoney() << "\n\n";
    }
    else{
        std::cout << "Properti ini akan masuk ke sistem lelang";
        for (int i = 0; i < 3; i++) {
            std::cout << ".";
            std::cout.flush(); 
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        std::cout << "\n";
    }
}
