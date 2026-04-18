#include "models/card/skill-card/ShieldCard.hpp"

ShieldCard::ShieldCard(const std::string& message) : SkillCard(message) {}

void ShieldCard::takeEffect(Player& p, GameManager& gm) {
    // Asumsi durasi tameng adalah 1 giliran
    p.setShielded(true, 1);
    std::cout << "[Shield] Tameng Aktif! Kamu kebal dari sewa dan pajak untuk 1 putaran ke depan.\n";
}