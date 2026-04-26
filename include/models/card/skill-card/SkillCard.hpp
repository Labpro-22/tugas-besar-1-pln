#pragma once
#include "models/card/Card.hpp"
#include "models/effect/PlayerEffect.hpp"

class UseSkillCardView;
class Tile;

class SkillCard : public Card {
public:
    SkillCard(const std::string& message) : Card(message) {}
    virtual bool prepareUse(UseSkillCardView& view, GameManager& gm)
    {
        (void)view;
        (void)gm;
        return true;
    }
    virtual int getCardValue() const { return 0; }
};