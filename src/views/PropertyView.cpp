#include "core/GameManager.hpp"
#include "views/PropertyView.hpp"
#include "models/tile/PropertyTile.hpp"
#define SPACE 30
void PropertyView::outputProperty() const{
    std::cout << "Masukkan kode petak: ";
    std::string tileCode;
    std::cin >> tileCode;
    const std::map<std::string, Tile*>& m = gameManager.getBoard().getMapTilesCodeTile();
    auto it = m.find(tileCode);
    if (it == m.end()) {
        std::cout << "Petak " << tileCode << " tidak ditemukan atau bukan properti.\n\n";
        return;
    }

    auto pt = dynamic_cast<PropertyTile*>(it->second);
    if (pt == nullptr || pt->getProperty() == nullptr) {

        std::cout << "Petak " << tileCode << " tidak ditemukan atau bukan properti.\n\n";
    }
    else{
        Property *pr = pt->getProperty();
        auto centerText = [&](const std::string& text) {
            int padding = WIDTH - text.length();
            int left = padding / 2;
            int right = padding - left;
            return std::string(left, ' ') + text + std::string(right, ' ');
        };
        std::cout << "+" << std::string(WIDTH, '=') << "+\n";
        std::cout << "|" <<  centerText("AKTA KEPEMILIKAN") << "|\n";
        std::cout << "|" << centerText("[" + pr->getColor() +"] " + pr->getName() + " (" + pr->getCode() + ")") <<"|\n";
        std::cout << "+" << std::string(WIDTH, '=') << "+\n";
        std::cout << "| " << std::setw(COLON_WIDTH) << " Harga Beli" << std::setw(WIDTH-COLON_WIDTH) << ": M" + std::to_string( pr->getPrice())<<"|\n";
        std::cout << "| " << std::setw(COLON_WIDTH) << " Nilai Gadai" << std::setw(WIDTH-COLON_WIDTH) << ": M" + std::to_string( pr->getMortgageValue())<<"|\n";
        std::cout << "+ " <<std::string(WIDTH, '-') << "+\n";
        if(auto sp = dynamic_cast<StreetProperty*>(pr)){
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
        }
        else if(auto rp = dynamic_cast<RailroadProperty*>(pr)){
            std::map<int, long long> rent = rp->getRent();
            for(int i = 0; i < (int)rent.size(); i++){
                std::cout << "|" << std::setw(COLON_WIDTH) << (" Sewa (" + std::to_string(i) + " Railroad dimiliki)") << std::setw(WIDTH-COLON_WIDTH) <<
                (": M" + std::to_string(rent[i])) << "|\n";
            }
        }
        else if(auto up = dynamic_cast<UtilityProperty*>(pr)){
            std::map<int,long long> rent = up->getRentMultiplier();
            for(int i = 0; i < (int)rent.size(); i++){
                std::cout << "|" << std::setw(COLON_WIDTH) << (" Sewa (" + std::to_string(i) + " Utility dimiliki)") << std::setw(WIDTH-COLON_WIDTH) <<
                (": " + std::to_string(rent[i])) << "|\n|" << std::setw(WIDTH) << std::right<<  " kali lipat jumlah hasil lemparan dadu|" << "\n";
            }
        }
        std::cout << "+" << std::string(WIDTH, '=') << "+\n";
        std::cout << "|" << " Status : ";
        switch (pr->getState())
        {
        case OWNED:
            std::cout << "OWNED";
            break;
        case MORTGAGED:
            std::cout << "MORTGAGED [M]";
            std::cout << "Harga tebus : M" << pr->redemptionPrice() << "\n";
            break;
        case BANK:
            std::cout << "BANK";
            break;
        default:
            break;
        }
        if (pr->getOwner() != nullptr) {
            std::cout << " (Pemain " << pr->getOwner()->getUsername() << ")";
        }
        else {
            std::cout << " (BANK)";
        }
        std::cout << "\n";
        std::cout << "+" << std::string(WIDTH, '=') << "+\n\n";

    }
}

void PropertyView::outputPlayerProperties() const{
    Player& p = gameManager.getCurrentPlayer();
    if(p.getProperties().empty()){
        std::cout << "Kamu belum memiliki properti apapun.\n\n";
    }
    else{
        std::vector<Property*> playerProperties = p.getProperties(); 

        std::sort(playerProperties.begin(), playerProperties.end(),
            [](Property* a, Property* b) {

                auto sa = dynamic_cast<StreetProperty*>(a);
                auto sb = dynamic_cast<StreetProperty*>(b);

                if (sa && !sb) return true;
                if (!sa && sb) return false;

                if (sa && sb) {
                    return sa->getColor() < sb->getColor();
                }

                return a->getPropertyType() < b->getPropertyType();
            }
        );
        std::string last = "";
        long long totalAsset = 0;
        std::cout << "=== Properti Milik: Pemain " << p.getUsername() << " ===\n\n";
        for(auto pr : playerProperties){
            if(auto sp = dynamic_cast<StreetProperty*>(pr)){
                if(last != sp->getColor()){
                    std::cout <<"[" << sp->getColor() <<"]\n";
                    last = sp->getColor();
                }
                std::cout << "  - " << std::setw(SPACE)  << (sp->getName() +  " (" + sp->getCode() +")");
                if(sp->hasHotel()){
                    std::cout << "Hotel  "; 
                }
                else if(sp->getHouseCount() > 0){
                    std::cout << sp->getHouseCount() << " rumah";
                }
            }
            else {
                if(pr->getPropertyType() == "RAILROAD"){
                    if(last != "STASIUN"){
                        last = "STASIUN";
                        std::cout << "\n[STASIUN]\n";
                    }
                }
                else{
                    if(last != "UTILITY"){
                        last = "UTILITY";
                        std::cout << "\n[UTILITY]\n";
                    }
                }
                std::cout << "  - " << std::setw(SPACE)  << (pr->getName() +  " (" + pr->getCode() +")");
            }
            std::cout << "M" << pr->calculateAssetValue() << "\t";
            switch (pr->getState())
            {
            case OWNED:
                std::cout << "OWNED\n";
                break;
            case MORTGAGED:
                std::cout << "MORTGAGED  [M]\n";
                break;
            default:
                std::cout << "\n";
                break;
            }
            totalAsset += pr->calculateAssetValue();
        }
        std::cout << "\nTotal kekayaan properti: M" << totalAsset << "\n\n";
    }
}

void PropertyView::outputRent(Property &pr)const{
    if(pr.isMortgaged()){
        std::cout<<"Properti ini sedang digadaikan [M]. Tidak ada sewa yang dikenakan.\n\n";
    }
    else{
        int width = pr.getOwner()->getUsername().length() + 15;
        Player& p = gameManager.getCurrentPlayer();
        if (p.hasEffect("SHIELD")) {
            std::cout << "[SHIELD ACTIVE]: Efek ShieldCard melindungi anda. Tidak perlu membayar sewa.\n";
            return;
        }
        long long money = p.getMoney(), rent = pr.calculateRent();
        std::cout << std::setw(width) << "Kondisi" << ": ";
        if(auto sp = dynamic_cast<StreetProperty*>(&pr)){
            std::cout << sp->getHouseCount() << " rumah\n"; 
        }
        else if(auto up = dynamic_cast<UtilityProperty*>(&pr)){
            std::cout << up->getOwner()->getUtilityPropertyCount() << " Properti Utility dimiliki\n";
        }
        else if(auto rp = dynamic_cast<RailroadProperty*>(&pr)){
            std::cout << rp->getOwner()->getRailroadPropertyCount() << " Properti Railroad dimiliki\n";
        }
        std::cout << std::setw(width) << "Sewa" << ": M" << rent << "\n\n";
        std::cout << std::setw(width) << "Uang kamu" << ": M" << money;
        if (p.hasEffect("DISCOUNT")) {
            rent *= (100 - p.getEffectValue("DISCOUNT")) / 100;
            std::cout << std::setw(width) << "[DISCOUNT ACTIVE] Tagihan sewa menjadi" << ": M" << rent << "\n\n";
        }
        if(money >= rent){
            std::cout << " -> M" << money -rent <<"\n";
            std::cout << std::setw(width) << ("Uang Pemain "+ pr.getOwner()->getUsername()) << ": M" << pr.getOwner()->getMoney() << "  -> M" << pr.getOwner()->getMoney() + rent << "\n\n";
        }
        else{
            std::cout << "Kamu tidak mampu membayar sewa penuh! (M" << rent << ")Uang kamu saat ini: " << money << "\n\n";
        }
    }
}
