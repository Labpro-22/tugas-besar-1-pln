#pragma once

#include "include/models/card/Card.hpp"

class CommunityChestCard : public Card {
public:
    CommunityChestCard(const std::string& message) : Card(message) {}
};