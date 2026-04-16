#include "GoToNearestStationCard.hpp"

GoToNearestStationCard::GoToNearestStationCard() {
    message =  "Pindah ke stasiun terdekat!"
}

void GoToNearestStationCard::takeEffect(Player& p){
    PlayerPiece& piece = p.getPiece(); 
    Board* board = piece.getBoard();

    if (board == nullptr) {
        message = "error: board bernilai null"
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
                break'
            }
        }
    }

    if (targetPos != -1) piece.setPosition(targetPos);
}