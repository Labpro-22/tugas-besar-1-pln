#include "core/GameManager.hpp"
#include "views/FestivalView.hpp"

Property* FestivalView::promptChooseProperty(std::vector<Property*> pr) const{
    std::cout <<"Daftar properti milikmu:\n";
    std::map<std::string, int> propertyIndex;
    int idx = 1;
    for(auto prop : pr){
        std::cout << "\t- " << prop->getCode() << " (" << prop->getName() << ")\n";
        propertyIndex[prop->getCode()] = idx++;
    }
    std::string code;
    std::cout<< " Masukkan kode properti: ";
    while(true){
        std::cin >> code;
        if(propertyIndex[code] != 0)break;
        std::cout<<"-> Kode Properti tidak valid atau bukan milikmu!\n";
    }

    return pr[propertyIndex[code]-1];
}
void FestivalView::outputFestivalStatus(Property& pr) const{
    long long mult = pr.getFestivalMultiplier();
    if(mult == 2){
        std::cout<<"Efek festival aktif!\n\nSewa awal: M" << pr.calculateRent() / 2 << "\nSewa sekarang: M" <<pr.calculateRent() << 
        "\nDurasi: 3 giliran\n";
    }
    else if(mult == 4 || mult == 8){
        if(mult < 8){
            std::cout<<"Efek diperkuat!\n\nSewa sebelumnya: M" << pr.calculateRent() / 2 << "\nSewa sekarang: M" <<pr.calculateRent() << 
            "\nDurasi di-reset menjadi: 3 giliran\n";
        }
        else{
            std::cout << "Efek sudah maksimum (harga sewa sudah digandakan tiga kali)\n\nDurasi di-reset menjadi: 3 giliran\n";
        }
    }
}