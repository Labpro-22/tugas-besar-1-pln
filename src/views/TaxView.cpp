#include "views/TaxView.hpp"
#include "core/GameManager.hpp"
int TaxView::promptIncomeTax(long long flat, double percentage ) const{

    int input;
    while(true){
        std::cout << "Pilih opsi pembayaran pajak:\n";
        std::cout << "1. Bayar flat M" << flat << "\n";
        std::cout << "2. Bayar " << percentage << "% dari total kekayaan\n(Pilih sebelum menghitung kekayaan!)\nPilihan (1/2): ";
        std::cin >> input ;
        if(input == 1 || input == 2){
            std::cout << "\n\n";
            Player& p = gameManager.getCurrentPlayer();
            long long money = p.getMoney();
            double tax = 0;
            if(input == 1){
                tax = flat;
                if(tax > money){
                    std::cout << "Kamu tidak mampu membayar pajak flat M" << tax << "!\nUang kamu saat ini: M" << money <<"\n\n";
                }
                else{
                    std::cout << std::setw(COLON_WIDTH) <<"Uang kamu" << ": M" << money<< "  -> M" << money - tax << "\n\n";
                }
            }
            else if(input == 2){
                std::cout <<"Total kekayaan kamu:\n";
                std::cout << std::setw(COLON_WIDTH) << "- Uang tunai" << ": M" << money << "\n";
                std::cout << std::setw(COLON_WIDTH) << "- Harga beli properti" << ": M";
                long long propertyBuyPriceTotal = 0;
                for(auto pr : p.getProperties()){
                    propertyBuyPriceTotal += pr->getPrice();
                }
                std::cout << propertyBuyPriceTotal << " (termasuk yang digadaikan)\n";
                std::cout << std::setw(COLON_WIDTH) << "- Harga beli bangunan" << ": M"; 
                long long buildingBuyPriceTotal = 0;
                for(auto pr : p.getProperties()){
                    if(pr->getPropertyType() == "STREET"){
                        buildingBuyPriceTotal += pr->calculateAssetValue() - pr->getPrice();
                    }
                }
                std::cout << buildingBuyPriceTotal << "\n";
                long long total = money + propertyBuyPriceTotal + buildingBuyPriceTotal;
                tax = std::abs(percentage*total/100);
                std::cout << std::setw(COLON_WIDTH) << "Total" << ": M" << total << "\n";
                std::cout << std::setw(COLON_WIDTH) << ("Pajak " + std::to_string(percentage) + "%") << ": M" << tax << "\n";
                std::cout << std::setw(COLON_WIDTH) <<"Uang kamu" << ": M" << money;
                if(tax > money){
                    std::cout << "\nKamu tidak mampu membayar pajak persentase M" << tax << "!\n\n";
                }
                else{
                    std::cout << "  -> M" << money - tax << "\n\n";
                }
            }
            return input;
        }
        std::cout <<"\nMasukan tidak valid!\n";
    }
}
void TaxView::outputLuxuryTax(long long tax)const{
    std::cout << "Pajak sebesar M" << tax << " langsung dipotong.\n";
    long long money = gameManager.getCurrentPlayer().getMoney();
    if(money >= tax){
        std::cout << "Uang kamu: M" << money << " -> M" << money - tax << "\n\n";
    }
    else{
        std::cout <<"Kamu tidak mampu membayar pajak!\nUang kamu saat ini: " << money << "\n\n";
    }
}