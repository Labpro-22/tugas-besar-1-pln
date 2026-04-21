#pragma once

#include <vector>
#include "Property.hpp"

class UtilityProperty: public Property {
    public:
        UtilityProperty(const std::string& code, const std::string& name, const std::string& color, long long price, long long mortgageValue, long long festivalMultiplier, int festivalDuration, std::vector<long long> rentMul);

        long long calculateRent() noexcept override;

        PropertyType getPropertyType() const noexcept override;

        long long calculateSellValue() const override;

        long long calculateAssetValue() const override;

        std::vector<long long> getRentMultiplier() const noexcept;
        
    private:
        std::vector<long long> rentMultiplier;
};