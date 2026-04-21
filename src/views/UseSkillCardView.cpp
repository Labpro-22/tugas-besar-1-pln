#include "views/UseSkillCardView.hpp"
#include "models/Player.hpp"
#include "models/GameManager.hpp"
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
    int pos;
    std::cout << "Pilih ID petak tujuan (0 - 39): ";
    std::cin >> pos;
    return pos;
}

Player* UseSkillCardView::askForTargetPlayer(GameManager& gm) {
    std::string name;
    std::cout << "Masukkan nama pemain lawan yang ingin ditarik: ";
    std::cin >> name;
    return gm.getPlayerByName(name);
}

int UseSkillCardView::askForDemolitionTileId() {
    int tileId;
    std::cout << "Masukkan ID petak properti yang ingin dihancurkan: ";
    std::cin >> tileId;
    return tileId;
}