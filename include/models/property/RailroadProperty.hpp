#pragma once

#include "Property.hpp"
#include <map>
#include <string>

class RailroadProperty: public Property {
    public:
        RailroadProperty(const std::string& code,const std::string& name,const std::string& color, long long price, long long mortgageValue, long long festivalMultiplier, int festivalDuration, std::map<int, long long> rentPrice);

        long long calculateRent() noexcept override;

        std::string getPropertyType() const noexcept override;

        long long calculateSellValue() const override;

        long long calculateAssetValue() const override;

        std::vector<long long> getRent() const noexcept;

        
    private: 
        std::map<int, long long>rent;
};