#include "core/GameManager.hpp"
#include "views/BuildView.hpp"
#define SPACE 30
StreetProperty* BuildView::promptChooseProperty(std::vector<Property*> pr) const{

    //bool cek apakah ada setidaknya satu set color group
    bool playerHaveAPropertySet = false;
    //map mengelompokkan color group dengan colorGroup[colour] = {streetprop1, streetprop2, dst}
    std::map<std::string, std::vector<StreetProperty*>> colorGroup;
    //Filter streetproperty dari property
    std::vector<StreetProperty*> streetpr;
    for(auto p : pr){
        if(auto sp = dynamic_cast<StreetProperty*>(p)){
            streetpr.push_back(sp);
        }
    }
    //insert ke map colorGroup
    for(auto prop : streetpr){
        if(gameManager.getCurrentPlayer().isPropertySetComplete(prop->getColor(),gameManager.getBoard())){
            if(!playerHaveAPropertySet){
                playerHaveAPropertySet =true;
            }
            colorGroup[prop->getColor()].push_back(prop);
        }
    }
    //output tidak punya satupun set colorgroup
    if(!playerHaveAPropertySet){
        std::cout << "Tidak ada color group yang memenuhi syarat untuk dibangun.\nKamu harus memiliki seluruh petak dalam satu color group terlebih dahulu.\n";
        return nullptr;
    }
    else{
        std::vector<std::string> colors;
        int idx = 1;
        for(const auto &[color, prop] : colorGroup){
            std::cout << idx << ".[" << color << "]\n";
            idx++;
            std::string s = "a. ";
            colors.push_back(color);
            for(const auto &p : prop){
                std::cout<<"\t" << std::left << std::setw(SPACE) <<  s + p->getName() + " (" + p->getCode() + ")" << ": ";
                if(p->getHouseCount() < 4){
                    std::cout<<p->getHouseCount() << " rumah (Harga rumah: M" << p->getHousePrice() << ")\n";
                }
                if(p->getHouseCount() == 4){
                     std::cout<<p->getHouseCount() << " rumah (Harga hotel: M" << p->getHotelPrice() << ")\n";
                }
                if(p->hasHotel()){
                    std::cout << "Hotel  (sudah maksimal, tidak dapat dibangun)\n";
                }
                s[0]++;

            }
        }
        std::cout << "Uang kamu saat ini: M" << gameManager.getCurrentPlayer().getMoney() << "\n";
        std::cout << "Pilih nomor color group (0 untuk batal): ";
        int inputIdx;
        std::cin >> inputIdx;
        if(inputIdx == 0)return nullptr;
        std::cout<<"\nCColor group [" << colors[idx-1] << "] :\n";
        idx = 1;
        std::vector<StreetProperty*> validBuildings;

        int minHouse = 6;

        for (const StreetProperty* p : colorGroup[colors[idx-1]]) {
            minHouse = std::min(minHouse, p->getHouseCount());
        }

        for(auto p  : colorGroup[colors[idx-1]]){
            if(p->getHouseCount() == minHouse)validBuildings.push_back(p);
        } 
        for(auto p  : validBuildings){
            std::cout<< idx << std::left << std::setw(SPACE) << ". " + p->getName() + " (" + p->getCode() + ")" << ": ";
            if(p->canBuildHouse(1)){
                std::cout<<p->getHouseCount() << " rumah (Harga rumah: M" << p->getHousePrice() << ")\n";
            }
            if(p->canBuildHotel()){
                    std::cout<<p->getHouseCount() << " rumah (Harga hotel: M" << p->getHotelPrice() << ")\n";
            }
            if(p->hasHotel()){
                std::cout << "Hotel  (sudah maksimal, tidak dapat dibangun)\n";
            }
            idx++;
        }
        std::cout << "Pilih nomor properti (0 untuk batal): ";
        std::cin >> inputIdx;
        if(inputIdx == 0)return nullptr;

        StreetProperty* toBuild = validBuildings[inputIdx];
        if(toBuild->hasHotel()){
            std::cout << "Properti sudah memiliki hotel, tidak dapat dibangun!\n";
            return nullptr;
        } 
        if(toBuild->canBuildHotel()){
            std::string yayOrNay;
            while(std::cin >> yayOrNay){
                std::cout<<"Upgrade " << toBuild->getName() << " ke hotel? Biaya: M" << toBuild->getHotelPrice() << " (y/n): ";
                if(yayOrNay == "y"){
                    std::cout << "\n\n";
                    break;
                }
                if(yayOrNay == "n"){
                    std::cout << "\n\n";
                    return nullptr;
                } 
                std::cout << "\nMasukan tidak valid!\n";
            }
        }
        return toBuild;
    }
}

void BuildView::outputBuildStatus(bool success, StreetProperty* pr) const{
    if(success){
        if(pr->hasHotel()){
            std::cout << pr->getName() << " di-upgrade ke Hotel!\n";
        }
        else{
            std::cout << "Kamu membangun 1 rumah di " << pr->getName()<< ". Biaya: M" << pr->getHousePrice() << "\n";
        }
    }
    else{
        std::cout << "Gagal membangun rumah / hotel!\n";
    }
}

