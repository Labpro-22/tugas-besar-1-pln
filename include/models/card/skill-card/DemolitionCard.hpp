#pragma once
#include "SkillCard.hpp"

class DemolitionCard : public SkillCard {
private:
    int targetTileId = -1;

public:
    DemolitionCard(const std::string &message);
    void setTargetTileId(int id);
    void takeEffect(Player &p, GameManager &gm) override;
    bool prepareUse(UseSkillCardView &view, GameManager &gm) override;
    std::string getCardType() const override;
};
