#include "GoToJailCard.hpp"

GoToJailCard::GoToJailCard() {
    message = "Pindah ke penjara!";
}

void GoToJailCard::takeEffect(Player& p){
    p.goToJail();
}