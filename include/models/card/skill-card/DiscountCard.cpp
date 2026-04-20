#include "models/card/skill-card/DiscountCard.hpp"
#include <random>

DiscountCard::DiscountCard(const std::string& message, int percentage) 
    : SkillCard(message), percentage(percentage) {}

void DiscountCard::takeEffect(Player& p, GameManager& gm) {
    p.applyDiscount(percentage, 1);
    std::cout << "Diskon " << percentage << "% aktif untuk 1 putaran!\n";
}