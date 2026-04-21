#include "models/property/RailroadProperty.hpp"


std::string RailroadProperty::getPropertyType() const noexcept {
    return "RAILROAD";
}

RailroadProperty::RailroadProperty(const std::string& code,const std::string& name,const std::string& color, long long price, long long mortgageValue, long long festivalMultiplier, int festivalDuration, std::vector<long long> rentPrice): Property(code, name, color, price, mortgageValue, festivalMultiplier, festivalDuration) {
    rent = rentPrice;
}

long long RailroadProperty::calculateRent() noexcept {
    if (owner == nullptr || isMortgaged()) return 0;
    int count = owner->getRailroadPropertyCount();
    if (count <= 0) return 0;
    if (count > (int) rent.size())  {
        count = rent.size();
    }
    long long baseRent = rent[count - 1];
    if (getFestivalDuration() > 0) {
        return baseRent * getFestivalMultiplier();
    }
    return baseRent;
}

long long RailroadProperty::calculateSellValue() const {
    return getPrice();
}

long long RailroadProperty::calculateAssetValue() const {
    return getPrice();
}