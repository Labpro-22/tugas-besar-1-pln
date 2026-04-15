#include "MoveCard.hpp"
#include "../../../Player.hpp"
#include "../../../PlayerPiece.hpp"
#include <random>

MoveCard::MoveCard(const std::string& message) : SkillCard(message) {
    rerandomizeAmount();
}

void MoveCard::rerandomizeAmount() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 12); // TODO : adjust the range of randomizer
    amount = dist(gen);
}

void MoveCard::takeEffect(Player& p) {
    if (p.getPiece() != nullptr) {
        p.getPiece()->goForward(amount);
    }
}