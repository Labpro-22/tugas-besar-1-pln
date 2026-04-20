#include "models/card/skill-card/ShieldCard.hpp"

ShieldCard::ShieldCard(const std::string& message) : SkillCard(message) {}

void ShieldCard::takeEffect(Player& p, GameManager& gm) {
    p.setShielded(true, 1);
    std::cout << "Tameng Aktif! Kebal dari bayar sewa/pajak untuk 1 putaran.\n";
}