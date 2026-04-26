#include "core/GameManager.hpp"
#include "views/FestivalView.hpp"

Property* FestivalView::promptChooseProperty(std::vector<Property*> pr) const{
    if(pr.empty()){
        std::cout << "Kamu belum memiliki properti. Efek festival dilewati.\n\n";
        return nullptr;
    }

    std::cout <<"Daftar properti milikmu:\n";
    std::map<std::string, Property*> propertyByCode;
    for(auto prop : pr){
        std::cout << "\t- " << prop->getCode() << " (" << prop->getName() << ")\n";
        propertyByCode[prop->getCode()] = prop;
    }
    std::cout << "\t- 0 (batal)\n";

    std::string code;
    std::cout<< " Masukkan kode properti: ";
    while(true){
        std::cin >> code;
        if(code == "0"){
            std::cout << "\n\n";
            return nullptr;
        }
        auto it = propertyByCode.find(code);
        if(it != propertyByCode.end()){
            return it->second;
        }
        std::cout<<"-> Kode Properti tidak valid atau bukan milikmu!\n";
    }
}
void FestivalView::outputFestivalStatus(Property& pr, long long previousMultiplier) const{
    if(previousMultiplier >= 8){
        std::cout << "Efek sudah maksimum (harga sewa sudah digandakan tiga kali)\n\nDurasi di-reset menjadi: 3 giliran\n";
    }
    else if(pr.getFestivalMultiplier() == 2){
        std::cout<<"Efek festival aktif!\n\nSewa awal: M" << pr.calculateRent() / 2 << "\nSewa sekarang: M" <<pr.calculateRent() <<
        "\nDurasi: 3 giliran\n";
    }
    else{
        std::cout<<"Efek diperkuat!\n\nSewa awal: M" << pr.calculateRent() / 2 << "\nSewa sekarang: M" <<pr.calculateRent() <<
        "\nDurasi: 3 giliran\n";
    }
}
