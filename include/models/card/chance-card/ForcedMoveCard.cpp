#include "ForcedMoveCard.hpp"

static std::string generateForcedMoveMessage(int offset) {
    if (offset < 0) return "Mundur " + std::to_string(std::abs(offset)) + " langkah.";
    if (offset > 0) return "Maju " + std::to_string(offset) + " langkah.";
    return "Tidak terjadi apa-apa.";
}

ForcedMoveCard::ForcedMoveCard(int moveOffset) 
    : ChanceCard(generateForcedMoveMessage(moveOffset)), moveOffset(moveOffset) {}

void ForcedMoveCard::takeEffect(Player& p, GameManager& gm) {
    if (p.getPiece() != nullptr) {
        p.getPiece()->goForward(moveOffset);
        std::cout << getMessage() << "\n";
        
        int currentPos = p.getPiece()->getPosition();
        gm.getBoard()->getTile(currentPos)->onLanded(&p, &gm);
    }
}