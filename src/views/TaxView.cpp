#include "views/TaxView.hpp"
#include "core/GameManager.hpp"
#include <limits>
int TaxView::promptIncomeTax(long long flat, double percentage ) const{
    return promptIncomeTax(flat, percentage, gameManager.getCurrentPlayer());
}

int TaxView::promptIncomeTax(long long flat, double percentage, Player &p) const{
    if (p.hasEffect("SHIELD")) {
        std::cout << "[SHIELD ACTIVE]: Efek ShieldCard melindungi anda. Tidak perlu membayar pajak.\n";
        return -1;
    }
    auto applyDiscount = [&](long long amount) {
        if (p.hasEffect("DISCOUNT")) {
            return amount * (100 - p.getEffectValue("DISCOUNT")) / 100;
        }
        return amount;
    };
    int input;
    while(true){
        std::cout << "Pilih opsi pembayaran pajak:\n";
        std::cout << "1. Bayar flat M" << flat << "\n";
        std::cout << "2. Bayar " << percentage << "% dari total kekayaan\n(Pilih sebelum menghitung kekayaan!)\nPilihan (1/2): ";
        if(!(std::cin >> input)){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout <<"\nMasukan tidak valid!\n";
            continue;
        }
        if(input == 1 || input == 2){
            std::cout << "\n\n";
            long long money = p.getMoney();
            long long tax = 0;
            if(input == 1){
                tax = applyDiscount(flat);
                if (p.hasEffect("DISCOUNT")) {
                    std::cout << "[DISCOUNT ACTIVE] Pajak flat menjadi: M" << tax << "\n";
                }
                if(tax > money){
                    std::cout << "Kamu tidak mampu membayar pajak flat M" << tax << "!\nUang kamu saat ini: M" << money <<"\n\n";
                }
                else{
                    std::cout << std::left << std::setw(COLON_WIDTH) <<"Uang kamu" << ": M" << money<< "  -> M" << money - tax << "\n\n";
                }
            }
            else if(input == 2){
                std::cout <<"Total kekayaan kamu:\n";
                std::cout << std::left << std::setw(COLON_WIDTH) << "- Uang tunai" << ": M" << money << "\n";
                std::cout << std::left << std::setw(COLON_WIDTH) << "- Harga beli properti" << ": M";
                long long propertyBuyPriceTotal = 0;
                for(auto pr : p.getProperties()){
                    propertyBuyPriceTotal += pr->getPrice();
                }
                std::cout << propertyBuyPriceTotal << " (termasuk yang digadaikan)\n";
                std::cout << std::left << std::setw(COLON_WIDTH) << "- Harga beli bangunan" << ": M"; 
                long long buildingBuyPriceTotal = 0;
                for(auto pr : p.getProperties()){
                    if(pr->getPropertyType() == "STREET"){
                        buildingBuyPriceTotal += pr->calculateAssetValue() - pr->getPrice();
                    }
                }
                std::cout << buildingBuyPriceTotal << "\n";
                long long total = money + propertyBuyPriceTotal + buildingBuyPriceTotal;
                long long originalTax = static_cast<long long>(std::abs(percentage*total/100));
                tax = applyDiscount(originalTax);
                std::cout << std::left << std::setw(COLON_WIDTH) << "Total" << ": M" << total << "\n";
                std::cout << std::left << std::setw(COLON_WIDTH) << ("Pajak " + std::to_string(percentage) + "%") << ": M" << originalTax << "\n";
                if (p.hasEffect("DISCOUNT")) {
                    std::cout << std::left << std::setw(COLON_WIDTH) << "[DISCOUNT ACTIVE] Pajak menjadi" << ": M" << tax << "\n";
                }
                std::cout << std::left << std::setw(COLON_WIDTH) <<"Uang kamu" << ": M" << money;
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
    outputLuxuryTax(tax, gameManager.getCurrentPlayer());
}

void TaxView::outputLuxuryTax(long long tax, Player &p)const{
    if (p.hasEffect("SHIELD")) {
        std::cout << "[SHIELD ACTIVE]: Efek ShieldCard melindungi anda. Tidak perlu membayar pajak.\n";
        return;
    }
    long long discountedTax = tax;
    if (p.hasEffect("DISCOUNT")) {
        discountedTax = tax * (100 - p.getEffectValue("DISCOUNT")) / 100;
    }
    std::cout << "Pajak sebesar M" << tax << " langsung dipotong.\n";
    if (p.hasEffect("DISCOUNT")) {
        std::cout << "[DISCOUNT ACTIVE] Pajak menjadi: M" << discountedTax << "\n";
    }
    long long money = p.getMoney();
    if(money >= discountedTax){
        std::cout << "Uang kamu: M" << money << " -> M" << money - discountedTax << "\n\n";
    }
    else{
        std::cout <<"Kamu tidak mampu membayar pajak!\nUang kamu saat ini: " << money << "\n\n";
    }
}
