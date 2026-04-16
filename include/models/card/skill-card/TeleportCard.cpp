#include "TeleportCard.hpp"
#include "../../../Player.hpp"

TeleportCard::TeleportCard(const std::string& message) : SkillCard(message) {}

void TeleportCard::takeEffect(Player& p) {
    // TODO : create set pending teleport
    p.setPendingTeleport(true); 
}