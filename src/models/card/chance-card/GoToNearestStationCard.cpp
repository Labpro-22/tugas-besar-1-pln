#include "GoToNearestStationCard.hpp"
#include "include/models/PlayerPiece.hpp"

GoToNearestStationCard::GoToNearestStationCard(const std::string& message) 
    : ChanceCard(message) {}

void GoToNearestStationCard::takeEffect(Player& p, GameManager& gm) {
    if (p.getPiece() != nullptr) {
        int currentPos = p.getPiece()->getPosition();
        // TODO : adjust for custom range
        int targetStation = (((currentPos + 5) / 10) * 10 + 5) % 40;
        
        p.getPiece()->setPosition(targetStation);
        std::cout << "Anda berpindah ke stasiun terdekat di petak " << targetStation << "!\n";
        
        gm.getBoard()->getTile(targetStation)->onLanded(&p, &gm);
    }
}