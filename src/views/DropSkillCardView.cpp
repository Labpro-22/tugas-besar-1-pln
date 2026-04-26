#include "views/DropSkillCardView.hpp"
#include "core/GameManager.hpp"
#include "models/card/skill-card/SkillCard.hpp"
#include <iostream>
#include <limits>

int DropSkillCardView::promptChooseSkillCard(const std::vector<SkillCard *> &cards)
{
    std::cout << "\n[Warning] Tangan Anda penuh! (Maksimal 3 kartu)\n";
    if (cards.empty()) {
        std::cout << "Kamu tidak memiliki Skill Card untuk dibuang.\n\n";
        return -1;
    }

    std::cout << "Pilih kartu yang ingin dibuang untuk memberi ruang:\n";

    for (size_t i = 0; i < cards.size(); ++i) {
        std::cout << i + 1 << ". " << cards[i]->getCardType();
        if (!cards[i]->getMessage().empty()) {
            std::cout << " - " << cards[i]->getMessage();
        }
        std::cout << "\n";
    }

    int choice;
    while (true) {
        std::cout << "Masukkan nomor kartu yang dibuang (1 - " << cards.size() << ", 0 untuk batal): ";
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

void DropSkillCardView::outputDropSkillCardStatus(SkillCard &card)
{
    std::cout << card.getCardType() << " telah dibuang.\n\n";
}
