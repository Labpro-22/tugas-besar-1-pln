#include "models/property/UtilityProperty.hpp"
#include "models/player/Player.hpp"

UtilityProperty::UtilityProperty(const std::string& code, const std::string& name, const std::string& color, long long price, long long mortgageValue, long long festivalMultiplier, int festivalDuration, std::map<int, long long> rentMul)
    : Property(code, name, color, price, mortgageValue, festivalMultiplier, festivalDuration) {
    rentMultiplier = rentMul;
}

long long UtilityProperty::calculateRent() const noexcept {
    if (owner == nullptr || isMortgaged()) return 0; 
    
    int count = owner->getUtilityPropertyCount();
    
    auto it = rentMultiplier.find(count);
    long long baseMultiplier = (it != rentMultiplier.end()) ? it->second : 0;
    
    return getFestivalDuration() != 0 ? getFestivalMultiplier() * baseMultiplier : baseMultiplier; 
}

std::string UtilityProperty::getPropertyType() const noexcept {
    return "UTILITY";
}

long long UtilityProperty::calculateSellValue() const noexcept {
    return getMortgageValue();
}

long long UtilityProperty::calculateAssetValue() const noexcept {
    return getMortgageValue();
}

std::map<int, long long> UtilityProperty::getRentMultiplier() const noexcept {
    return rentMultiplier;
}

long long UtilityProperty::redemptionPrice() const noexcept {
    return 2 * getMortgageValue();
}

void UtilityProperty::sellProperty() {
    resetOwnerAsBank();
}
