#include "models/card/chance-card/ForcedMoveCard.hpp"

static std::string ForcedMoveCard::generateForcedMoveMessage(int offset) {
    if (offset < 0) return message = "Mundur " + std::to_string(std::abs(offset)) + " langkah.";
    if (offset > 0) return message = "Maju " + std::to_string(offset) + " langkah.";
    return message = "Tidak terjadi apa-apa.";
}

ForcedMoveCard::ForcedMoveCard(int moveOffset) 
    : ChanceCard(generateForcedMoveMessage(moveOffset)), moveOffset(moveOffset) {}

void ForcedMoveCard::takeEffect(Player& p, GameManager& gm) {
    if (p.getPiece() != nullptr) {
        p.getPiece()->goForward(moveOffset);
        std::cout << getMessage() << "\n";
        
        int currentPos = p.getPiece()->getPosition();
        gm.getBoard()->getTile(currentPos)->onLanded(&p, &gm);

        generateForcedMoveMessage(moveOffset);
    }
}