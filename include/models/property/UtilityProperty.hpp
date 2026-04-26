#pragma once

#include <map>
#include "Property.hpp"

class UtilityProperty: public Property {
    public:
        UtilityProperty(const std::string& code, const std::string& name, const std::string& color, long long price, long long mortgageValue, long long festivalMultiplier, int festivalDuration, std::map<int, long long> rentMul);

        long long calculateRent() const noexcept override;

        std::string getPropertyType() const noexcept override;

        long long calculateSellValue() const noexcept override;

        long long calculateAssetValue() const noexcept override;

        std::map<int, long long> getRentMultiplier() const noexcept;

        long long redemptionPrice() const noexcept override;

        void sellProperty() override;
        
    private:
        std::map<int, long long> rentMultiplier;
};