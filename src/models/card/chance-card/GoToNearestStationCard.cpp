#include "models/card/chance-card/GoToNearestStationCard.hpp"
#include "models/player/PlayerPiece.hpp"
#include "models/player/Player.hpp"
#include "core/GameManager.hpp"

GoToNearestStationCard::GoToNearestStationCard(const std::string& message) 
    : ChanceCard(message) {}

void GoToNearestStationCard::takeEffect(Player& p, GameManager& gm) {
    int currentPos = p.getPiece().getPosition();
    // TODO : adjust for custom range
    int targetStation = (((currentPos + 5) / 10) * 10 + 5) % 40;
    
    if (targetStation < currentPos) {
        gm.getBoard().getTile(0)->onPassBy(p, gm);
    }
    p.getPiece().setPosition(targetStation);
    message = "Anda berpindah ke stasiun terdekat di petak " + std::to_string(targetStation) + "!";

    Tile* tile = gm.getBoard().getTile(targetStation);
    if (tile) tile->onLanded(p, gm);
}

std::string GoToNearestStationCard::getCardType() const {
    return "GOTONEARESTSTATIONCARD";
}
