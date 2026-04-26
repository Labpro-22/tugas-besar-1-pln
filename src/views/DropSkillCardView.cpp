#include "views/DropSkillCardView.hpp"
#include "core/GameManager.hpp"
#include "models/card/skill-card/SkillCard.hpp"
#include <iostream>

int DropSkillCardView::promptChooseSkillCard(const std::vector<SkillCard *> &cards)
{
    std::cout << "\n[Warning] Tangan Anda penuh! (Maksimal 3 Kartu)\n";
    std::cout << "Pilih kartu yang ingin dibuang untuk memberi ruang:\n";

    for (size_t i = 0; i < cards.size(); ++i) {
        std::cout << i + 1 << ". " << cards[i]->getMessage() << "\n";
    }

    int choice;
    std::cout << "Masukkan nomor (1 - " << cards.size() << "): ";
    std::cin >> choice;

    return choice - 1;
}

void DropSkillCardView::outputDropSkillCardStatus(SkillCard &card)
{
    std::cout << card.getCardType() << " telah dibuang. Sekarang kamu memiliki 3 kartu di tangan.\n\n";
}
