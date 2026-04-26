#include "models/card/chance-card/GoToNearestStationCard.hpp"
#include "models/player/PlayerPiece.hpp"
#include "models/player/Player.hpp"
#include "models/tile/PropertyTile.hpp"
#include "core/GameManager.hpp"

GoToNearestStationCard::GoToNearestStationCard(const std::string& message) 
    : ChanceCard(message) {}

void GoToNearestStationCard::takeEffect(Player& p, GameManager& gm) {
    int currentPos = p.getPiece().getPosition();
    const Board& board = gm.getBoard();
    int tileCount = board.getTileCount();

    // Find nearest railroad station by scanning forward from current position
    int targetStation = -1;
    for (int offset = 1; offset <= tileCount; offset++) {
        int pos = (currentPos + offset) % tileCount;
        Tile* tile = board.getTile(pos);
        if (tile == nullptr) continue;
        PropertyTile* propTile = dynamic_cast<PropertyTile*>(tile);
        if (propTile && propTile->getProperty()->getPropertyType() == "RAILROAD") {
            targetStation = pos;
            break;
        }
    }

    if (targetStation < 0) {
        message = "Tidak ada stasiun ditemukan di papan!";
        return;
    }

    if (targetStation <= currentPos) {
        Tile* startTile = board.getTile(board.getTilePosition("GO"));
        if (startTile) startTile->onPassBy(p, gm);
    }

    p.getPiece().setPosition(targetStation);
    message = "Anda berpindah ke stasiun terdekat di petak " + board.getTile(targetStation)->getName() + "!";

    Tile* tile = board.getTile(targetStation);
    if (tile) tile->onLanded(p, gm);
}

std::string GoToNearestStationCard::getCardType() const {
    return "GOTONEARESTSTATIONCARD";
}