#include "core/GameManager.hpp"
#include "views/BuyView.hpp"

bool BuyView::promptBuyProperty(Property& pr){
    Property *p = &pr;
    if (auto sp = dynamic_cast<StreetProperty*>(p)) {
        auto centerText = [&](const std::string& text) {
            int padding = WIDTH - text.length();
            int left = padding / 2;
            int right = padding - left;
            return std::string(left, ' ') + text + std::string(right, ' ');
        };
        std::cout << "+" << std::string(WIDTH, '=') << "+" ;
        std::cout << "|" <<  centerText("AKTA KEPEMILIKAN") << "|\n";
        std::cout << "|" << centerText("[" + sp->getColor() +"] " + sp->getName() + " (" + sp->getCode() + ")") <<"|\n";
        std::cout << "+" << std::string(WIDTH, '=') << "+" ;
        std::cout << "| " << std::setw(COLON_WIDTH) << " Harga Beli" << std::setw(WIDTH-COLON_WIDTH) << ": M" + std::to_string( sp->getPrice())<<"|\n";
        std::cout << "| " << std::setw(COLON_WIDTH) << " Nilai Gadai" << std::setw(WIDTH-COLON_WIDTH) << ": M" + std::to_string( sp->getMortgageValue())<<"|\n";
        std::cout << "+ " <<std::string(WIDTH, '-') << "+\n";
        std::array<long long, 6> rent= sp->getRentPrice();
        for(int i=0; i < 6; i++){
            if(i == 0){
                std::cout <<"|" << std::setw(COLON_WIDTH) << " Sewa (unimproved)" << std::setw(WIDTH-COLON_WIDTH) << (": M" + std::to_string(rent[0])) << "|\n";
            }
            else if(i > 0 && i < 6){
                std::cout << "|" << std::setw(COLON_WIDTH) << (" Sewa (" + std::to_string(i) + " rumah)") << std::setw(WIDTH-COLON_WIDTH) << (": M" + std::to_string(rent[i])) << "|\n";
            }
            else{
                std::cout<< "|" <<std::setw(COLON_WIDTH) << " Sewa (hotel)" << std::setw(WIDTH-COLON_WIDTH) << (": M" + std::to_string(rent[5])) << "|\n";
            }
        }
        std::cout << "+" << std::string(WIDTH,'-') << "+\n";
        std::cout << "|" << std::setw(COLON_WIDTH) << " Harga Rumah" << std::setw(WIDTH-COLON_WIDTH) << ( ": M" + std::to_string(sp->getHousePrice()))<<"|\n";
        std::cout << "|" <<std::setw(COLON_WIDTH) << " Harga Hotel" << std::setw(WIDTH-COLON_WIDTH) << (": M" + std::to_string(sp->getHotelPrice()))<<"|\n";
        std::cout << "+" << std::string(WIDTH, '=') << "+" ;


        std::cout << "Uang kamu saat ini: M" << gameManager.getCurrentPlayer().getMoney() << "\n";
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
    else if(auto up = dynamic_cast<UtilityProperty*>(p)){
        std::cout<<"Belum ada yang menginjaknya duluan, " << p->getName() << " kini menjadi milikmu!\n\n";
        return true;
    }
    else if(auto rp =  dynamic_cast<RailroadProperty*>(p)){
        std::cout<<"Belum ada yang menginjaknya duluan, stasiun ini kini menjadi milikmu!\n\n";
        return true;
    }
}

void BuyView::outputBuyStatus(bool success, Property* pr)const{
    if(success){
        std::cout << pr->getName() <<  " kini menjadi milikmu!\nUang tersisa: M" << gameManager.getCurrentPlayer().getMoney() << "\n\n";
    }
    else{
        std::cout << "Properti ini akan masuk ke sistem lelang";
        for (int i = 0; i < 3; i++) {
            std::cout << ".";
            std::cout.flush(); 
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
}