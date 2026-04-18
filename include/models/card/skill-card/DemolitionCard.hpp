#pragma once
#include "SkillCard.hpp"

class DemolitionCard : public SkillCard {
public:
    explicit DemolitionCard(const std::string& message);
    void takeEffect(Player& p,GameManager& gm) override;
};