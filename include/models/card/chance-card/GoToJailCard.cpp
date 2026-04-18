#include "GoToJailCard.hpp"

GoToJailCard::GoToJailCard() {
    message = "Pindah ke penjara!";
}

void GoToJailCard::takeEffect(Player& p,GameManager& gm) {
    cout << "[Kesempatan] Kamu dipindahkan ke penjara!\n";
    p.goToJail();
}