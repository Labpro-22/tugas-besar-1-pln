#pragma once


#include "PropertyException.hpp"
#include <string>

class Player;

typedef enum {
    BANK,
    OWNED,
    MORTGAGED
} PropertyState;

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

        virtual long long calculateRent() noexcept;
        
        void startFestival();

        long long getFestivalMultiplier() const noexcept;

        int getFestivalDuration() const noexcept;

        void onNextTurn();

        bool isMortgaged() const noexcept;

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