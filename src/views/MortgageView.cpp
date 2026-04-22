#include "core/GameManager.hpp"
#include "MortgageView.hpp"

Property* MortgageView::promptChooseProperty(std::vector<Property*> pr) const{
    std::cout<<"=== Properti yang Dapat Digadaikan ===\n";
    auto it = std::find_if(pr.begin(), pr.end(),
    [](Property* p) {
        return !p->isMortgaged();
    }
    );
    if(it == pr.end()){
        std::cout << "Tidak ada properti yang dapat digadaikan saat ini.\n";
        return nullptr;
    }
    else{
        int idx = 1, i = 0;
        std::map<int,int> indeks;
        for(auto p : pr){
            if(!p->isMortgaged()){
                indeks[idx] = i;
                std::cout << idx++ << ". " << p->getName() << 
                " (" << p->getCode() << ")" + FORMAT_SPACE_20(p->getCode().size() + p->getName().size())*' '
                << "[" << p->getColor() << "] Nilai Gadai: M" << p->getMortgageValue()<< "\n"; 
                
            }
            i++;
        }
        std::cout << "Pilih nomor properti (0 untuk batal): ";
        int input;
        while(true){
            std::cin >> input;
            if(input == 0){
                std::cout << "\n\n";
                return nullptr;
            }
            if(input < 0 || input >= idx){
                std::cout << "Nomor properti tidak Valid!\n";
            }
            else{
                return pr[indeks[input]];
            }
        }
    }
    
}
void MortgageView::outputMortgageStatus(bool success, Property &pr) const{
    if(success){
        std::cout << pr.getName() << " berhasil digadaikan\n";
        std::cout << "Kamu menerima M" << pr.getMortgageValue() << " dari Bank.\n";
        std::cout << "Uang kamu sekarang: M" << gameManager.getCurrentPlayer().getMoney() << "\n";
        std::cout << "Catatan: Sewa tidak dapat dipungut dari properti yang digadaikan.\n\n";
    }
    else{
        std::cout << pr.getName() << " tidak dapat digadaikan!\n";
        std::cout << "Masih terdapat bangunan di color group [" << pr.getColor()<< "].\nBangunan harus dijual terlebih dahulu.\n\n";
    }
} 
bool MortgageView::sellAllBuildings(std::vector<StreetProperty*>colorGroupProperty) {
    std::cout << "Daftar bangunan di color group [" << colorGroupProperty[0]->getColor() << "]:\n";
    int i = 1;
    for(auto sp : colorGroupProperty){
        std::cout << i << ". " << sp->getName() << " (" << sp->getCode() << ")" << FORMAT_SPACE_20(sp->getName().size())*' ' << " - ";
        if(sp->hasHotel()){
            std::cout << "Hotel -> Nilai jual bangunan : M" << sp->calculateSellValue() << "\n";
        }
        else{
            std::cout << sp->getHouseCount() <<" rumah  -> Nilai jual bangunan : M" << sp->calculateSellValue() - sp->getPrice()<<"\n";
            i++;
        }
    } 
    std::cout << "Jual semua bangunan color group ["  << colorGroupProperty[0]->getColor() << "]? (y/n)";
    while(true){
        std::string in;
        std::cin >> in;
        if(in == "y"){
            long long total = 0;
            for(auto sp : colorGroupProperty){
                std::cout << "Bangunan  " << sp->getName() << "terjual. Kamu menerima M"  << sp->calculateSellValue() - sp->getPrice()<< "\n\n";
            }
            std::cout << "Uang kamu sekarang: M" << gameManager.getCurrentPlayer().getMoney() + total << "\n\n";
            return true;
        if(in == "n")return false;
        std::cout << "Masukan tidak valid!\nJual semua bangunan color group ["   << colorGroupProperty[0]->getColor() << "]? (y/n)";
        }   
    }
}