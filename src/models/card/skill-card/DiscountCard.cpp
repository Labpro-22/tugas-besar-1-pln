#include "models/card/skill-card/DiscountCard.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"
#include <random>

DiscountCard::DiscountCard(const std::string& message, int percentage) 
    : SkillCard(message), percentage(percentage) {}

void DiscountCard::takeEffect(Player& p, GameManager& gm) {
    p.addEffect(PlayerEffect("DISCOUNT", false, 1, percentage));
    message = "Diskon " + std::to_string(percentage) + "% aktif untuk 1 putaran!";
}

std::string DiscountCard::getCardType() const {
    return "DiscountCard"; 
}