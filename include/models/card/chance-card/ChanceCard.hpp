#pragma once
#include "models/card/Card.hpp"


class ChanceCard : public Card {
public:
    ChanceCard(const std::string& message) : Card(message) {}
};