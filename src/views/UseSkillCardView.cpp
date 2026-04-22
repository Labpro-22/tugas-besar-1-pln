#include "views/UseSkillCardView.hpp"
#include "core/GameManager.hpp"
#include "models/card/skill-card/TeleportCard.hpp"
#include "models/card/skill-card/LassoCard.hpp"
#include "models/card/skill-card/DemolitionCard.hpp"
#include <iostream>
#include <string>

int UseSkillCardView::promptChooseCardToUse(const std::vector<SkillCard*>& cards) {
    if (cards.empty()) {
        std::cout << "Anda tidak memiliki Skill Card.\n";
        return -1;
    }

    std::cout << "\n=== Skill Card Anda ===\n";
    for (size_t i = 0; i < cards.size(); ++i) {
        std::cout << i + 1 << ". " << cards[i]->getMessage() << "\n";
    }
    std::cout << "0. Batal / Kembali\n";
    
    int choice;
    std::cout << "Pilih kartu yang ingin digunakan (0 - " << cards.size() << "): ";
    std::cin >> choice;
    
    return choice - 1; 
}

void UseSkillCardView::processAndPlayCard(SkillCard* card, Player& player, GameManager& gm) {
    std::cout << "\n--- Menggunakan Kartu ---\n";
    card->prepareUse(*this, gm);
    card->takeEffect(player, gm);
}

int UseSkillCardView::askForTargetPosition() {
    std::string tileCode;
    Board& b = gameManager.getBoard();
    while(true){
        std::cout << "Pilih kode petak tujuan: ";
        std::cin >> tileCode;
        if(b.getTilePosition(tileCode) != -1)return b.getTilePosition(tileCode);
        std::cout << "\nKode petak tidak valid!\n";
    }
}
Player* UseSkillCardView::askForTargetPlayer(GameManager& gm) {
    std::string name;

    while(true){
        std::cout << "Masukkan nama pemain lawan yang ingin ditarik (0 untuk batal): ";
        std::cin >> name;
        if(name == "0")return nullptr;
        for(auto &p : gm.getPlayers()){
            if(p.getUsername() == name)return &p;
        }
        std::cout<<"\nTidak ada player dengan nama " << name << "!\n";
    }
}

int UseSkillCardView::askForDemolitionTileId() {
    std::string tileCode;
    Board& b = gameManager.getBoard();
    while(true){
        std::cout << "Pilih kode petak tujuan: ";
        std::cin >> tileCode;
        if(b.getTilePosition(tileCode) != -1)return b.getTilePosition(tileCode);
        std::cout << "\nKode petak tidak valid!\n";
    }
}