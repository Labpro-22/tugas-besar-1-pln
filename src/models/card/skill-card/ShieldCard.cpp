#include "models/card/skill-card/ShieldCard.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"

ShieldCard::ShieldCard(const std::string& message) : SkillCard(message) {}

void ShieldCard::takeEffect(Player& p, GameManager&) {
    p.addEffect(PlayerEffect("SHIELD", false, 1, 0));
    message = "Shield anda aktif! Anda kebal dari bayar sewa/pajak sampai giliran aktif ini selesai.";
}

std::string ShieldCard::getCardType() const {
    return "ShieldCard"; 
}
