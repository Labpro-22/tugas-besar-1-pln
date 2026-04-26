#include "models/property/StreetProperty.hpp"
#include "models/player/Player.hpp"

StreetProperty::StreetProperty(const std::string& code, const std::string& name, const std::string& color, long long price, long long mortgageValue, long long festivalMultiplier, int festivalDuration, long long housePrice, long long hotelPrice, long long rent[6], int houseCount): Property(code, name, color, price, mortgageValue, festivalMultiplier, festivalDuration), housePrice(housePrice), hotelPrice(hotelPrice), houseCount(houseCount) {
    for (int i = 0; i < 6; i++) {
        rentPrice[i] = rent[i];
    }
} 

long long StreetProperty::calculateRent() const noexcept {
    if (owner == nullptr || isMortgaged()) return 0;
    return getFestivalDuration() != 0 ? getFestivalMultiplier() * rentPrice[houseCount] : rentPrice[houseCount]; 
}

int StreetProperty::getHouseCount() const noexcept {
    return houseCount;
}

void StreetProperty::buildHouse(int n) {
    if (!canBuildHouse(n)) {
        throw CantBuildException("Jumlah rumah yang dibuat melebihi kapasitas lahan!");
    }
    else {
        houseCount++;
    }
}

void StreetProperty::buildHotel() {
    if (hasHotel()) {
        throw CantBuildException("Sudah terdapat hotel dalam petak ini!");
    }
    if (!canBuildHotel()) {
       throw CantBuildException("Tidak dapat membangun hotel, jumlah rumah belum memenuhi!");
    }
    else {
        houseCount++;
    }
}
bool StreetProperty::hasHotel() const noexcept {
    return houseCount == 5;
}

bool StreetProperty::canBuildHouse(int n) const noexcept {
    if (isMortgaged() || n + houseCount > 4) return false;
    else return true;
}

bool StreetProperty::canBuildHotel() const noexcept{
    if (isMortgaged() || hasHotel()) return false;
    if (houseCount == 4) return true;
    else return false;
}

long long StreetProperty::getHousePrice() const noexcept {
    return housePrice;
}

long long StreetProperty::getHotelPrice() const noexcept {
    return hotelPrice;
}

void StreetProperty::removeBuilding() {
    if (houseCount <= 0) {
        throw CantBuildException("Tidak terdapat bangunan apa-apa di lahan ini!");
    }
    else {
        houseCount--;
    }
}

std::string StreetProperty::getPropertyType() const noexcept {
    return "STREET";
}

long long StreetProperty::calculateSellValue() const noexcept {
    long long sellValue = getPrice();

    int houses = getHouseCount();
    if (houses > 0 && houses <= 4) {
        sellValue += (houses * getHousePrice());
    } else if (houses == 5) {
        sellValue += (4 * getHousePrice() + getHotelPrice());
    }

    return sellValue;
}

long long StreetProperty::calculateAssetValue() const noexcept {
    long long sellValue = getPrice();

    int houses = getHouseCount();
    if (houses > 0 && houses <= 4) {
        sellValue += (houses * getHousePrice());
    } else if (houses == 5) {
        sellValue += (4 * getHousePrice() + getHotelPrice());
    }

    return sellValue;
}

std::array<long long, 6> StreetProperty::getRentPrice() const noexcept {
    return {rentPrice[0], rentPrice[1], rentPrice[2], rentPrice[3], rentPrice[4], rentPrice[5]};
}

long long StreetProperty::redemptionPrice() const noexcept {
    return getPrice();
}

void StreetProperty::sellProperty() {
    resetToBank();
}

void StreetProperty::resetToBank() noexcept {
    houseCount = 0;
    setMortgaged(false);
    resetOwnerAsBank();
}
