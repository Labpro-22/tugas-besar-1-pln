#pragma once
#include <string>

class PlayerEffect {
private:
    std::string name;
    bool isOneTimeUse;
    int duration;
    int value;

public:
    PlayerEffect(const std::string& name, bool isOneTimeUse, int duration, int value = 0);

    std::string getName() const;
    bool getIsOneTimeUse() const;
    int getDuration() const;
    int getValue() const;

    void decrementDuration();
    bool isExpired() const;
};