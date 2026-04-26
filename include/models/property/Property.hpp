#pragma once


#include "PropertyException.hpp"
#include <string>

class Player;

typedef enum {
    BANK,
    OWNED,
    MORTGAGED
} PropertyState;

// enum class PropertyType {
//     STREET,
//     RAILROAD,
//     UTILITY
// };

class Property {
    public:
        Property(const std::string& code,const std::string& name,const std::string& color, long long price, long long mortgageValue, long long festivalMultiplier, int festivalDuration);

        std::string getName() const noexcept;

        std::string getCode() const noexcept;

        Player* getOwner() const noexcept;

        PropertyState getState() const noexcept;

        void setOwner(Player *p) noexcept;

        void resetOwnerAsBank() noexcept;

        std::string getColor() const noexcept;

        long long getPrice() const noexcept;

        long long getMortgageValue() const noexcept;

        void setMortgaged(bool mortgaged) noexcept;

        virtual long long calculateRent() const noexcept = 0;

        virtual long long redemptionPrice() const noexcept = 0;
        
        void startFestival();

        long long getFestivalMultiplier() const noexcept;

        int getFestivalDuration() const noexcept;

        void onNextTurn();

        bool isMortgaged() const noexcept;

        virtual std::string getPropertyType() const noexcept = 0;

        virtual long long calculateSellValue() const noexcept = 0;
        
        virtual long long calculateAssetValue() const noexcept = 0;

        virtual ~Property() = default;

        virtual void sellProperty() = 0;

    protected:
        Player *owner;

    private:
        std::string code;

        std::string name;

        PropertyState state;

        std::string color;

        long long price;

        long long mortgageValue;

        long long festivalMultiplier;
        
        int festivalDuration;
};