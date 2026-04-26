#include "core/GameManager.hpp"
#include "views/UnmortgageView.hpp"
#include <limits>
#define SPACE 30
#define SPACE_2 20
Property* UnmortgageView::promptChooseProperty(std::vector<Property*> pr) const{
    std::cout<<"=== Properti yang Sedang Digadaikan ===\n";
    auto it = std::find_if(pr.begin(), pr.end(),
    [](Property* p) {
        return p->isMortgaged();
    }
    );
    if(it == pr.end()){
        std::cout << "Tidak ada properti yang sedang digadaikan.\n\n";
    }
    else{
        int idx = 1, i = 0;
        std::map<int,int> indeks;
        for(auto p : pr){
            if(p->isMortgaged()){
                indeks[idx] = i;
                std::cout << idx++ << ". " << std::left << std::setw(SPACE)<<p->getName() + 
                " (" + p->getCode() + ")" << std::left << std::setw(SPACE_2) <<  "[" + p->getColor() + "]" << "[M]  " <<  "Harga Tebus: M" << p->redemptionPrice()<< "\n"; 
            }
            i++;
        }
        std::cout << "Pilih nomor properti (0 untuk batal): ";
        int input;
        while(true){
            if(!(std::cin >> input)){
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Nomor properti tidak Valid!\n";
                continue;
            }
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
    return nullptr;

}

void UnmortgageView::outputUnmortgageStatus(bool success, Property &pr) const{
    if(success){
        std::cout << pr.getName() << " berhasil ditebus!\nKamu membayar M" << pr.redemptionPrice() << " ke Bank.\nUang kamu sekarang: M" 
        << gameManager.getCurrentPlayer().getMoney() << "\n\n";
    }
    else{
        std::cout << "Uang kamu tidak cukup untuk menebus "  << pr.getName() <<".\nHarga tebus: M" << pr.redemptionPrice() <<
         "| Uang kamu: M" << gameManager.getCurrentPlayer().getMoney() << "\n\n";
    }
}
