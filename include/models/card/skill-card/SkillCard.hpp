#pragma once
#include "../Card.hpp"

class SkillCard : public Card {
public:
    SkillCard(const std::string& message);
    virtual ~SkillCard() = default;
};