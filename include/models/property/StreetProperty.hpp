#pragma once

#include "Property.hpp"
#include <array>

class StreetProperty: public Property {
    public:
        StreetProperty(const std::string& code, const std::string& name, const std::string& color, long long price, long long mortgageValue, long long festivalMultiplier, int festivalDuration, long long housePrice, long long hotelPrice, long long rentPrice[6], int houseCount = 0);

        long long calculateRent() const noexcept override;

        int getHouseCount() const noexcept;

        void buildHouse(int n);
        
        void buildHotel();

        bool hasHotel() const noexcept;

        bool canBuildHouse(int n) const noexcept;

        bool canBuildHotel() const noexcept;

        long long getHousePrice() const noexcept;

        long long getHotelPrice() const noexcept;

        void removeBuilding();

        std::string getPropertyType() const noexcept override;

        long long calculateSellValue() const noexcept override;

        long long calculateAssetValue() const noexcept override;

        std::array<long long, 6> getRentPrice() const noexcept;

        long long redemptionPrice() const noexcept override;

        void sellProperty() override;

        void resetToBank() noexcept override;

    private:
        int houseCount;

        long long housePrice;

        long long hotelPrice;

        long long rentPrice[6];
};
