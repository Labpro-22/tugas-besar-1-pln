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
        std::cout << i + 1 << ". " << cards[i]->getCardType();
        if (!cards[i]->getMessage().empty()) {
            std::cout << " - " << cards[i]->getMessage();
        }
        std::cout << "\n";
    }
    std::cout << "0. Batal / Kembali\n";
    
    int choice;
    while (true) {
        std::cout << "Pilih kartu yang ingin digunakan (0 - " << cards.size() << "): ";
        std::cin >> choice;

        if (choice == 0) {
            return -1;
        }
        if (choice >= 1 && choice <= static_cast<int>(cards.size())) {
            return choice - 1;
        }

        std::cout << "Pilihan tidak valid.\n";
    }
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
