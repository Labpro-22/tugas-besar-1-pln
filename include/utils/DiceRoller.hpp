#pragma once

#include <random>

class DiceRoller {
private:
    static std::random_device rd_;
    static std::mt19937 rng_;
    static std::uniform_int_distribution<> dist_;
    static std::pair<int, int> lastRoll_;

public:
    static std::pair<int, int> roll();
    static std::pair<int, int> getLastRoll();
    static bool isLastRollDouble();
};