#include "models/card/skill-card/TeleportCard.hpp"

TeleportCard::TeleportCard(const std::string& message) : SkillCard(message) {}

void TeleportCard::takeEffect(Player& p, GameManager& gm) {
    int pos;
    std::cout << "[Teleport] Pilih ID petak tujuan (0 - 39): "; // TODO : adjust size
    std::cin >> pos;
    
    if ( pos >= 0 && pos < 40 && p.getPiece() != nullptr ) {
        p.getPiece()->setPosition(pos);
        std::cout << "Kamu berteleportasi ke petak " << pos << "!\n";
        gm.getBoard()->getTile(pos)->onLanded(&p, &gm);
    } 
    else {
        std::cout << "Petak tidak valid. Teleportasi gagal.\n";
    }
}