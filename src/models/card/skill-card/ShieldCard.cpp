#include "models/card/skill-card/ShieldCard.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"

ShieldCard::ShieldCard(const std::string& message) : SkillCard(message) {}

void ShieldCard::takeEffect(Player& p, GameManager& gm) {
    p.addEffect(PlayerEffect("SHIELD", false, 1, 0));
    message = "Shield anda aktif! anda kebal dari bayar sewa/pajak untuk 1 putaran.";
}

std::string ShieldCard::getCardType() const {
    return "ShieldCard"; 
}