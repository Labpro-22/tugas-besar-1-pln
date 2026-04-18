#include "GoToNearestStationCard.hpp"
#include "models/PlayerPiece.hpp"

GoToNearestStationCard::GoToNearestStationCard() {
    message =  "Pindah ke stasiun terdekat!";
}

void GoToNearestStationCard::takeEffect(Player& p,GameManager& gm) {
    PlayerPiece& piece = p.getPiece(); 
    Board* board = piece.getBoard();

    if (board == nullptr) {
        message = "error: board bernilai null";
        return;
    }

    int currentPos = piece.getPosition();
    int totalTiles = board->getTileCount();
    int targetPos = -1;

    for (int i = 1; i <= totalTiles; ++i) {
        int checkPos = (currentPos + i) % totalTiles;
        Tile* tile = board->getTile(checkPos);
        
        PropertyTile* propTile = dynamic_cast<PropertyTile*>(tile);
        if (propTile != nullptr) {
            
            Property* prop = propTile->getProperty();
            if (dynamic_cast<RailroadProperty*>(prop) != nullptr) {
                targetPos = checkPos;
                break;
            }
        }
    }

    cout << "[Kesempatan] Kamu dipindahkan ke stasiun terdekat!\n";
    if (targetPos != -1) piece.setPosition(targetPos);
}