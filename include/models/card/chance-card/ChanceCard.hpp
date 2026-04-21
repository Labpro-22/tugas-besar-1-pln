#pragma once
#include "models/card/Card.hpp"

// TODO : message attribute
class ChanceCard : public Card {
public:
    ChanceCard(const std::string& message) : Card(message) {}
};