#include "GoToJailCard.hpp"

GoToJailCard::GoToJailCard(const std::string& message) : ChanceCard(message) {}

void GoToJailCard::takeEffect(Player& p, GameManager& gm) {
    if (p.getPiece() != nullptr) {
        p.getPiece()->setPosition(10);
        p.setJailed(true);
        std::cout << "Anda dipindahkan kepenjara!\n";
    }
}