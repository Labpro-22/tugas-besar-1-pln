#pragma once

#include "SpecialTile.hpp"

class StartTile: public SpecialTile {
    public:
        StartTile(const std::string& name, const std::string& code, long long goSalary): Tile(name, code), SpecialTile(name, code), goSalary(goSalary) {}

        void onLanded(Player& p, GameManager& gm) override;
    
        void onPassBy(Player& p, GameManager& gm) override;
    
        private:
            long long goSalary;
};