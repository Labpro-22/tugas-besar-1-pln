#include "core/GameManager.hpp"
#include "views/BankruptView.hpp"
#define SPACE 20
void BankruptView::outputPotentialWealth(Player &p, long long debt) const{
    long double wealth = 0;
    std::cout<<"Uang kamu       : M" << p.getMoney() << "\n";
    std::cout<<"Total kewajiban : M" << debt << "\n";
    std::cout<<"Kekurangan      : M" << debt - p.getMoney() << "\n";
    std::cout<<"Estimasi dana maksimum dari likuidasi:\n";
    for(auto p: p.getProperties()){
        if(!p->isMortgaged()){
            std::cout << "\tJual" << p->getName() << " (" << p->getCode() << ")\t[" << p->getColor() << "]\t -> M" ;
            wealth += p->calculateSellValue();
            std::cout << p->calculateSellValue() << "\n";
        }
    }
    std::cout << "\tTotal potensi\t -> M " << wealth;
    if(wealth >= debt){
        std::cout << "Dana likuidasi dapat menutup kewajiban.\nKamu wajib melikuidasi aset untuk membayar.\n\n";
    }
    else{
        std::cout << "Tidak cukup untuk menutup kewajiban M" << debt <<".\n\n";
    }
}

std::pair<std::string, Property*> BankruptView::promptLiquidation(std::vector<Property*> pr, long long debt){
    std::cout << "=== Panel Likuidasi ===\n";
    std::cout << "Uang kamu saat ini: M" << gameManager.getCurrentPlayer().getMoney() << "  |  Kewajiban: M" << debt << "\n\n";

    std::vector<Property*> sellableList;
    std::vector<Property*> mortgageableList;

    std::cout << "[Jual ke Bank]\n";
    int i = 1;
    for(auto prop : pr){
        if(!prop->isMortgaged()){
            std::cout << i << ". " << prop->getName() << " (" << prop->getCode() << ")\t[" << prop->getColor() << "]\tHarga Jual: M" << prop->calculateSellValue();
            if(auto sp = dynamic_cast<StreetProperty*>(prop)){
                if(sp->hasHotel()){
                    std::cout << " (termasuk hotel)";
                } else if(sp->getHouseCount() > 0){
                    std::cout << " (termasuk " << sp->getHouseCount() << " Rumah)";
                }
            }
            std::cout << "\n";
            sellableList.push_back(prop);
            i++;
        }
    }

    int sellCount = i - 1;
    std::cout << "[Gadai]\n";
    for(auto prop : pr){
        if(!prop->isMortgaged()){
            std::cout << i << ". " << prop->getName() << " (" << prop->getCode() << ")\t[" << prop->getColor() << "]\tNilai Gadai: M" << prop->getMortgageValue();
            if(auto sp = dynamic_cast<StreetProperty*>(prop)){
                if(sp->hasHotel() || sp->getHouseCount() > 0){
                    std::cout << " (bangunan dijual dulu)";
                }
            }
            std::cout << "\n";
            mortgageableList.push_back(prop);
            i++;
        }
    }

    while(true){
        std::cout << "Pilih aksi (0 jika sudah cukup): ";
        int input;
        std::cin >> input;
        if(input == 0){
            return std::make_pair("Batal", nullptr);
        }
        if(input >= 1 && input <= sellCount){
            Property* p = sellableList[input - 1];
            std::cout << p->getName() << " terjual ke Bank. Kamu menerima M" << p->calculateSellValue();
            std::cout << ".\nUang kamu sekarang: M" << gameManager.getCurrentPlayer().getMoney() + p->calculateSellValue() << "\n";
            return std::make_pair("Jual", p);
        }
        if(input > sellCount && input < i){
            Property* p = mortgageableList[input - sellCount - 1];
            std::cout << p->getName() << " digadaikan ke Bank. Kamu menerima M" << p->getMortgageValue();
            std::cout << ".\nUang kamu sekarang: M" << gameManager.getCurrentPlayer().getMoney() + p->getMortgageValue() << "\n";
            return std::make_pair("Gadai", p);
        }
        std::cout << "Masukkan tidak valid!\n";
    }
}
void BankruptView::outputDebtPaid(long long debt, Player* creditor)const{
    std::cout << "Kewajiban M" << debt << " terpenuhi. Membayar ke" << creditor->getUsername() ;
    for (int i = 0; i < 3; i++) {
        std::cout << ".";
        std::cout.flush(); 
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::cout << "\nUang kamu: M" << gameManager.getCurrentPlayer().getMoney() + debt  << " -> M" <<  gameManager.getCurrentPlayer().getMoney() << "\n";
    std::cout << "Uang " << creditor->getUsername() << ": M" << creditor->getMoney() -debt<< " -> M" << creditor->getMoney() << "\n\n"; 
}

void BankruptView::outputBankruptByBank(Player &p) const{
    std::cout << p.getUsername() << " dinyatakan BANGKRUT!\nKreditor: Bank\n\n";
    std::cout << "Uang sisa: M" << p.getMoney() << "diserahkan ke Bank.\n";
    std::cout << "Seluruh properti dikembalikan ke status BANK.\nBangunan dihancurkan — stok dikembalikan ke Bank.\n\n";
    if(p.getProperties().size() > 0){
        std::cout << "Properti berikut akan dilelang:\n";
        for(auto pr : p.getProperties()){
            std::cout << "-> " << std::left << std::setw(SPACE) <<  pr->getName() <<  "(" << pr->getCode() << ")";
        }
    }
    std::cout << p.getUsername() << " telah keluar dari permainan.\n";
    if(gameManager.getPlayers().size() >1){
        std::cout << "Permainan berlanjut dengan " << gameManager.getPlayers().size()<< " pemain tersisa.\n";
    }
    std::cout << "Memasuki sistem lelang";
    for (int i = 0; i < 3; i++) {
        std::cout << ".";
        std::cout.flush(); 
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::cout <<"\n\n";
    
}

void BankruptView::outputBankruptByPlayer(Player &p, Player& creditor) const{
    std::cout << p.getUsername() << " dinyatakan BANGKRUT!\nKreditor: " << creditor.getUsername() << "\n\n";
    std::cout << "Pengalihan aset ke " << creditor.getUsername() << ":\n";
    std::cout << "   - Uang tunai sisa  : M" << p.getMoney()  << "\n";
    for(auto pr: p.getProperties()){
        std::cout <<std::left << std::setw(SPACE) << pr->getName() <<  "(" << pr->getCode() << ")\t" << std::left << std::setw(10) 
        << ("[" + pr->getColor() + "]") ;
        switch (pr->getState())
        {
        case OWNED:
            std::cout << "OWNED";
            break;
        case MORTGAGED:
            std::cout << "MORTGAGED";
            break;
        case BANK:
            std::cout << "BANK";
            break;
        default:
            break;
        }
        if(auto sp = dynamic_cast<StreetProperty*> (pr)){
            if(sp->hasHotel()){
                std::cout<<"( Hotel )";
            }
            else if(sp->getHouseCount() > 0){
                std::cout << "(" << sp->getHouseCount() << ")";
            }
        }
        std::cout <<"\n";
    }
    std::cout <<"\n" << creditor.getUsername() << "menerima semua aset " << p.getUsername() << ".\n"; 
    std::cout << p.getUsername() << " telah keluar dari permainan.\n";
    if(gameManager.getPlayers().size() >1){
        std::cout << "Permainan berlanjut dengan " << gameManager.getPlayers().size()<< " pemain tersisa.\n";
    }
    std::cout <<"\n\n";
}