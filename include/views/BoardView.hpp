#pragma once
#include "models/tile/PropertyTile.hpp"
#include "models/tile/special-tile/StartTile.hpp"
#include "models/tile/special-tile/JailTile.hpp"
#include "models/tile/special-tile/GoToJailTile.hpp"
#include "models/tile/special-tile/FreeParkingTile.hpp"
#include "models/tile/special-tile/FestiveTile.hpp"
#include "models/tile/special-tile/tax-tile/LuxuryTaxTile.hpp"
#include "models/tile/special-tile/tax-tile/IncomeTaxTile.hpp"
#include "models/tile/special-tile/card-tile/ChanceCardTile.hpp"
#include "models/tile/special-tile/card-tile/CommunityChestTile.hpp"
class GameManager;

class BoardView
{
private:
    GameManager& gameManager;
    void drawTile(bool left, bool top, bool right, bool bottom) const;
public:
    BoardView(GameManager& gm) : gameManager(gm) {}
    void drawBoard() const;
    void outputOnLanded() const;
    void outputOnLanded(Player &p) const;
    void outputOnPassByStart() const;
};


