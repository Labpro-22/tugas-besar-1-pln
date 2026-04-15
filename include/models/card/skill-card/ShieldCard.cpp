#include "ShieldCard.hpp"
#include "../../../Player.hpp"

ShieldCard::ShieldCard(const std::string& message) : SkillCard(message) {}

void ShieldCard::takeEffect(Player& p) {
    // Asumsi class Player memiliki method setShielded
    p.setShielded(true); 
}