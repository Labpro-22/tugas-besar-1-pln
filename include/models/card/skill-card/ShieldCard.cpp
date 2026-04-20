#include "models/card/skill-card/ShieldCard.hpp"

ShieldCard::ShieldCard(const std::string& message) : SkillCard(message) {}

void ShieldCard::takeEffect(Player& p, GameManager& gm) {
    p.addEffect(PlayerEffect("SHIELD", false, 1, 0));
    std::cout << "Shield anda aktif! anda kebal dari bayar sewa/pajak untuk 1 putaran.\n";
}