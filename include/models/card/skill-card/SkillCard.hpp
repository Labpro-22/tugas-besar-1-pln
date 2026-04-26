#pragma once
#include "models/card/Card.hpp"
#include "models/effect/PlayerEffect.hpp"

class UseSkillCardView;
class Tile;

class SkillCard : public Card {
public:
    SkillCard(const std::string& message) : Card(message) {}
    virtual void prepareUse(UseSkillCardView& view, GameManager& gm) {};
    virtual int getCardValue() const { return 0; }
};