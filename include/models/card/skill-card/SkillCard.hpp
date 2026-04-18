#pragma once
#include "models/card/Card.hpp"

class SkillCard : public Card {
public:
    SkillCard(const std::string& message);
    virtual ~SkillCard() = default;
};