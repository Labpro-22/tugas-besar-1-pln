#include "models/card/skill-card/DiscountCard.hpp"
#include <random>

DiscountCard::DiscountCard(const std::string& message) : SkillCard(message) {
    rerandomizePercentage();
}

void DiscountCard::rerandomizePercentage() {
    std::random_device rd;
    std::mt19937 gen(rd());

    // TODO : adjust randomizer range
    std::uniform_int_distribution<> dist(1, 5);
    percentage = dist(gen) * 10; 
}

void DiscountCard::takeEffect(Player& p, GameManager& gm) {
    p.applyDiscount(percentage, 1); 
    std::cout << "[Discount] Kamu mengaktifkan Diskon " << percentage << "% untuk 1 putaran ke depan!\n";
}