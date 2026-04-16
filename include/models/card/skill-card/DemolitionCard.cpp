#include "DemolitionCard.hpp"
#include "../../../Player.hpp"

DemolitionCard::DemolitionCard(const std::string& message) : SkillCard(message) {}

void DemolitionCard::takeEffect(Player& p) {
    p.setPendingDemolition(true);
}