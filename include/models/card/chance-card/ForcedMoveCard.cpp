#include "ForcedMoveCard.hpp"

ForcedMoveCard::ForcedMoveCard(const int moveOffset) : moveOffset(moveOffset) {
    message = "[Kesempatan] "
    if ( moveOffset < 0 ) message += "Mundur " + std::to_string(moveOffset) + " langkah.";
    else if( moveOffset > 0 ) message += "Maju " + std::to_string(moveOffset) + " langkah.";
    else message += "Tidak terjadi apa-apa!";
}

void ForcedMoveCard::takeEffect(Player& p,GameManager& gm) {
    cout<<getMessage()<<"\n";
    player.getPiece().goBackward(moveOffset);
}