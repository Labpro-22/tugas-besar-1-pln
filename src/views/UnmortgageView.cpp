#include "core/GameManager.hpp"
#include "views/UnmortgageView.hpp"
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
                std::cout << idx++ << ". " << std::setw(SPACE)<<p->getName() + 
                " (" + p->getCode() + ")" << std::setw(SPACE_2) <<  "[" + p->getColor() + "]" << "[M]  " <<  "Harga Tebus: M" << p->getPrice()<< "\n"; 
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
    return nullptr;

}

void UnmortgageView::outputUnmortgageStatus(bool success, Property &pr) const{
    if(success){
        std::cout << pr.getName() << " berhasil ditebus!\nKamu membayar M" << pr.getPrice() << " ke Bank.\nUang kamu sekarang: M" 
        << gameManager.getCurrentPlayer().getMoney() << "\n\n";
    }
    else{
        std::cout << "Uang kamu tidak cukup untuk menebus "  << pr.getName() <<".\nHarga tebus: M" << pr.getPrice() <<
         "| Uang kamu: M" << gameManager.getCurrentPlayer().getMoney() << "\n\n";
    }
}