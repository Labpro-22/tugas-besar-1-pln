#include "ShieldCard.hpp"
#include "../../../Player.hpp"

ShieldCard::ShieldCard(const std::string& message) : SkillCard(message) {}

void ShieldCard::takeEffect(Player& p) {
    // TODO : create p set shielded
    p.setShielded(true); 
}