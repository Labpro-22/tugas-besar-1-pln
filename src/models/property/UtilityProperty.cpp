#include "models/property/UtilityProperty.hpp"
#include "models/player/Player.hpp"

UtilityProperty::UtilityProperty(const std::string& code, const std::string& name, const std::string& color, long long price, long long mortgageValue, long long festivalMultiplier, int festivalDuration, std::map<int, long long> rentMul): Property(code, name, color, price, mortgageValue, festivalMultiplier, festivalDuration) {
    rentMultiplier = rentMul;
}


long long UtilityProperty::calculateRent() noexcept {
    if (owner == nullptr || isMortgaged) return 0;
    return getFestivalDuration() != 0 ? getFestivalMultiplier() * rentMultiplier[owner->getUtilityPropertyCount()] : rentMultiplier[owner->getUtilityPropertyCount()]; 
}

std::string UtilityProperty::getPropertyType() const noexcept {
    return "UTILITY";
}

long long UtilityProperty::calculateSellValue() const {
    return getPrice();
}

long long UtilityProperty::calculateAssetValue() const {
    return getPrice();
}

std::vector<long long> UtilityProperty::getRentMultiplier() const noexcept {
    return rentMultiplier;
}
