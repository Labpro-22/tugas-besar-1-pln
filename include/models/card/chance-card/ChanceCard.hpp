#pragma once
#include "include/models/card/Card.hpp"
#include "include/models/Player.hpp"

// TODO : message attribute
class ChanceCard : public Card {
public:
    ChanceCard(const std::string& message) : Card(message) {}
};