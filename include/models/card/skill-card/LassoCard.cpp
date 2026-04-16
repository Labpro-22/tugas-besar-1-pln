#include "LassoCard.hpp"
#include "../../../Player.hpp"
#include "../../../PlayerPiece.hpp"

LassoCard::LassoCard(const std::string& message) : SkillCard(message) {}

void LassoCard::takeEffect(Player& p) {
    // TODO : create set pending lasso
    p.setPendingLasso(true); 
}