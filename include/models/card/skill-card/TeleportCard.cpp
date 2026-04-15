#include "TeleportCard.hpp"
#include "../../../Player.hpp"

TeleportCard::TeleportCard(const std::string& message) : SkillCard(message) {}

void TeleportCard::takeEffect(Player& p) {
    // Catatan: Karena takeEffect hanya menerima (Player& p), 
    // input posisi target idealnya di-set ke atribut kartu ini *sebelum* takeEffect dipanggil,
    // atau di-handle via flag agar GameManager yang memindahkan.
    
    // Pemicu status teleport untuk dieksekusi GameManager/View
    p.setPendingTeleport(true); 
}