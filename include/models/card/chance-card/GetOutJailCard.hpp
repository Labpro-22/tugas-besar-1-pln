#pragma once

#include "ChanceCard.hpp"

// Kayaknya ga dibutuhkan 
class GetOutJailCard : public ChanceCard {
public:
    void takeEffect(Player& player) override;
}