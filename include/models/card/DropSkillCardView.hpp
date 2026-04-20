#pragma once
#include <vector>

class SkillCard;

class DropSkillCardView {
public:
    int promptChooseCardToDrop(const std::vector<SkillCard*>& cards);
    void showDropResult(bool success);
};