#include "views/UseSkillCardView.hpp"
#include "core/GameManager.hpp"
#include "models/card/skill-card/DiscountCard.hpp"
#include "models/card/skill-card/DemolitionCard.hpp"
#include "models/card/skill-card/LassoCard.hpp"
#include "models/card/skill-card/MoveCard.hpp"
#include "models/card/skill-card/TeleportCard.hpp"
#include "models/tile/PropertyTile.hpp"
#include <iostream>
#include <limits>
#include <string>

namespace {
std::string getCardDescription(const SkillCard &card)
{
    if (const auto *moveCard = dynamic_cast<const MoveCard *>(&card)) {
        return "Maju " + std::to_string(moveCard->getAmount()) + " langkah lalu jalankan efek petak tujuan.";
    }
    if (const auto *discountCard = dynamic_cast<const DiscountCard *>(&card)) {
        return "Diskon " + std::to_string(discountCard->getPercentage()) + "% untuk transaksi 1 putaran.";
    }
    if (card.getCardType() == "ShieldCard") {
        return "Kebal dari pajak, sewa, dan hukuman finansial selama 1 putaran.";
    }
    if (card.getCardType() == "TeleportCard") {
        return "Pindah ke petak mana pun yang kamu pilih.";
    }
    if (card.getCardType() == "LassoCard") {
        return "Tarik satu lawan ke petak tempatmu berada.";
    }
    if (card.getCardType() == "DemolitionCard") {
        return "Hancurkan satu properti yang dimiliki pemain dan kembalikan ke bank.";
    }
    return card.getMessage().empty() ? "Belum ada deskripsi kartu." : card.getMessage();
}
}

int UseSkillCardView::promptChooseCardToUse(const std::vector<SkillCard*>& cards) {
    if (cards.empty()) {
        std::cout << "Anda tidak memiliki Skill Card.\n";
        return -1;
    }

    std::cout << "\n=== Skill Card Anda ===\n";
    for (size_t i = 0; i < cards.size(); ++i) {
        std::cout << i + 1 << ". " << cards[i]->getCardType() << "\n";
        std::cout << "   " << getCardDescription(*cards[i]) << "\n";
    }
    std::cout << "0. Batal / Kembali\n";
    
    int choice;
    while (true) {
        std::cout << "Pilih kartu yang ingin digunakan (0 - " << cards.size() << "): ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Pilihan tidak valid.\n";
            continue;
        }

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
    if (card->prepareUse(*this, gm)) {
        card->takeEffect(player, gm);
    }
}

void UseSkillCardView::outputCardUseResult(const SkillCard& card) const {
    std::cout << "\n[KARTU DIGUNAKAN] " << card.getCardType() << "\n";
    std::cout << card.getMessage() << "\n\n";
}

void UseSkillCardView::outputCardUseCancelled(const SkillCard& card) const {
    std::cout << "\nPenggunaan " << card.getCardType() << " dibatalkan. Kartu tidak jadi dipakai.\n\n";
}

void UseSkillCardView::outputReceivedCard(const SkillCard& card) const {
    std::cout << "\n[KARTU BARU] " << card.getCardType() << "\n";
    std::cout << getCardDescription(card) << "\n\n";
}

int UseSkillCardView::askForTargetPosition() {
    std::string tileCode;
    Board& b = gameManager.getBoard();
    while(true){
        std::cout << "Pilih kode petak tujuan (0 untuk batal): ";
        std::cin >> tileCode;
        if (tileCode == "0") {
            return -1;
        }
        if(b.getTilePosition(tileCode) != -1)return b.getTilePosition(tileCode);
        std::cout << "\nKode petak tidak valid!\n";
    }
}
Player* UseSkillCardView::askForTargetPlayer(GameManager& gm) {
    std::string name;

    while(true){
        bool foundPlayer = false;
        std::cout << "Masukkan nama pemain lawan yang ingin ditarik (0 untuk batal): ";
        std::cin >> name;
        if(name == "0")return nullptr;
        for(auto &p : gm.getPlayers()){
            if(p.getUsername() == name) {
                foundPlayer = true;
                if (&p == &gm.getCurrentPlayer()) {
                    std::cout << "\nKamu tidak bisa menargetkan dirimu sendiri.\n";
                    break;
                }
                if (p.isBankrupt()) {
                    std::cout << "\nPemain tersebut sudah bangkrut dan tidak bisa ditargetkan.\n";
                    break;
                }
                if (p.isJailed()) {
                    std::cout << "\nPemain tersebut sedang di penjara dan tidak bisa ditargetkan LassoCard.\n";
                    break;
                }
                return &p;
            }
        }
        if (!foundPlayer) {
            std::cout<<"\nTidak ada player dengan nama " << name << "!\n";
        }
    }
}

int UseSkillCardView::askForDemolitionTileId() {
    std::string tileCode;
    Board& b = gameManager.getBoard();
    while(true){
        std::cout << "Pilih kode properti yang ingin dihancurkan (0 untuk batal): ";
        std::cin >> tileCode;
        if (tileCode == "0") {
            return -1;
        }

        int tileId = b.getTilePosition(tileCode);
        if (tileId == -1) {
            std::cout << "\nKode petak tidak valid!\n";
            continue;
        }

        Tile *tile = b.getTile(tileId);
        auto *propertyTile = dynamic_cast<PropertyTile *>(tile);
        if (propertyTile == nullptr || propertyTile->getProperty() == nullptr) {
            std::cout << "\nPetak tersebut bukan properti.\n";
            continue;
        }
        if (propertyTile->getProperty()->getOwner() == nullptr) {
            std::cout << "\nProperti tersebut masih milik bank, jadi tidak perlu dihancurkan.\n";
            continue;
        }
        return tileId;
    }
}
