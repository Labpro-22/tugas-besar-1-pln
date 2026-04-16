#pragma once

#include "Property.hpp"

class StreetProperty: public Property {
    public:
        StreetProperty(const std::string& code, const std::string& name, const std::string& color, long long price, long long mortgageValue, long long festivalMultiplier, int festivalDuration, long long housePrice, long long hotelPrice, long long rentPrice[6]);

        long long calculateRent() noexcept override;

        int getHouseCount() const noexcept;

        void buildHouse(int n);
        
        void buildHotel();

        bool hasHotel() const noexcept;

        bool canBuildHouse(int n) const noexcept;

        bool canBuildHotel() const noexcept;

        long long getHousePrice() const noexcept;

        long long getHotelPrice() const noexcept;

        void removeBuilding();

    private:
        int houseCount;

        long long housePrice;

        long long hotelPrice;

        long long rentPrice[6];
};