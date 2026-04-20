#pragma once
#include "include/models/card/Card.hpp"

class SkillCard : public Card {
public:
    SkillCard(const std::string& message) : Card(message) {}
};