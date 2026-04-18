#include "models/card/skill-card/LassoCard.hpp"

LassoCard::LassoCard(const std::string& message) : SkillCard(message) {}

void LassoCard::takeEffect(Player& p, GameManager& gm) {
    std::string targetUsername;
    std::cout << "[Lasso] Masukkan nama pemain yang ingin ditarik: ";
    std::cin >> targetUsername;
    
    Player* targetP = gm.getPlayerByName(targetUsername); 
    
    if (targetP != nullptr && targetP->getPiece() != nullptr) {
        int currentPos = p.getPiece()->getPosition();
        targetP->getPiece()->setPosition(currentPos);
        std::cout << "Pemain " << targetUsername << " ditarik ke petakmu!\n";
    } else {
        std::cout << "Pemain tidak ditemukan atau nama salah.\n";
    }
}