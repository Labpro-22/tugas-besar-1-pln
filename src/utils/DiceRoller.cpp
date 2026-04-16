#pragma once

#include "core/DiceRoller.hpp"

std::random_device DiceRoller::rd_;
std::mt19937 DiceRoller::rng_{rd_()};
std::uniform_int_distribution<> DiceRoller::dist_{1, 6};
std::pair<int, int> DiceRoller::lastRoll_{0, 0}; 

std::pair<int, int> DiceRoller::roll() {
    return lastRoll_ = {dist_(rng_), dist_(rng_)};
}
std::pair<int, int> DiceRoller::getLastRoll() {
    return lastRoll_;
}
bool DiceRoller::isLastRollDouble() {
    return lastRoll_.first == lastRoll_.second && lastRoll_.first != 0;
}