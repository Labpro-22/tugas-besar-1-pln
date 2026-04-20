#include "models/effect/PlayerEffect.hpp"

PlayerEffect::PlayerEffect(const std::string& name, bool isOneTimeUse, int duration, int value)
    : name(name), isOneTimeUse(isOneTimeUse), duration(duration), value(value) {}

std::string PlayerEffect::getName() const { return name; }
bool PlayerEffect::getIsOneTimeUse() const { return isOneTimeUse; }
int PlayerEffect::getDuration() const { return duration; }
int PlayerEffect::getValue() const { return value; }

void PlayerEffect::decrementDuration() {
    if (!isOneTimeUse && duration > 0) {
        duration--;
    }
}

bool PlayerEffect::isExpired() const {
    if (isOneTimeUse) return false; 
    return duration <= 0;
}