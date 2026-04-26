#include "models/property/Property.hpp"
#include "models/player/Player.hpp"

Property::Property(const std::string& code, const std::string& name, const std::string& color, long long price, long long mortgageValue, long long festivalMultiplier, int festivalDuration)
    : owner(nullptr), code(code), name(name), state(BANK), color(color),
      price(price), mortgageValue(mortgageValue), 
      festivalMultiplier(festivalMultiplier), festivalDuration(festivalDuration) {}

std::string Property::getName() const noexcept {
    return name;
}

std::string Property::getCode() const noexcept {
    return code;
}

Player* Property::getOwner() const noexcept {
    return owner;
}

PropertyState Property::getState() const noexcept {
    return state;
}

void Property::setOwner(Player *p) noexcept {
    owner = p;
    state = OWNED;
}

void Property::resetOwnerAsBank() noexcept {
    owner = nullptr;
    state = BANK;
}

std::string Property::getColor() const noexcept {
    return color;
}

long long Property::getPrice() const noexcept {
    return price;
}

long long Property::getMortgageValue() const noexcept {
    return mortgageValue;
}

void Property::setMortgaged(bool mortgaged) noexcept {
    if (mortgaged) {
        state = MORTGAGED;
    }
    else {
        state = OWNED;
    }
}

void Property::resetToBank() noexcept {
    resetOwnerAsBank();
}

void Property::startFestival() {
    festivalDuration = 3;
    if (festivalMultiplier < 8) {
        festivalMultiplier *= 2;
    }
}

long long Property::getFestivalMultiplier() const noexcept {
    return festivalMultiplier;
}

int Property::getFestivalDuration() const noexcept {
    return festivalDuration;
}

void Property::onNextTurn() {
    if (festivalDuration > 0) {
        festivalDuration--;
    }
}

bool Property::isMortgaged() const noexcept {
    return (state == MORTGAGED);
}
