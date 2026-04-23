#include "core/GameManager.hpp"
#include "views/BoardView.hpp"

void BoardView::outputOnLanded()const{
    Player &p = gameManager.getCurrentPlayer();
    Tile* t = p.getPiece().getCurrentTile();
    if(auto pt = dynamic_cast<PropertyTile*>(t)){
        std::cout << "Kamu mendarat di " << pt->getName() << "(" + pt->getProperty()->getCode() + ")";
        Player *owner = pt->getProperty()->getOwner();
        if( owner != nullptr){
            if(owner->getUsername() != p.getUsername()){
                std::cout <<", milik Pemain " << owner->getUsername();
            }
        }
    }
    else{
        std::cout << "Kamu mendarat di petak " << t->getName();
    }
    std::cout << "!\n\n";
}
void BoardView::outputOnLanded(Player &p)const{
    Tile* t = p.getPiece().getCurrentTile();
    if(auto pt = dynamic_cast<PropertyTile*>(t)){
        std::cout << "Pemain " + p.getUsername() <<" mendarat di " << pt->getName() << "(" + pt->getProperty()->getCode() + ")";
        Player *owner = pt->getProperty()->getOwner();
        if( owner != nullptr){
            if(owner->getUsername() != p.getUsername()){
                std::cout <<", milik Pemain " << owner->getUsername();
            }
        }
    }
    else{
        std::cout << "Pemain " + p.getUsername()<<  " mendarat di petak " << t->getName();
    }
    std::cout << "!\n\n";
}

void BoardView::outputOnPassByStart()const{
    std::cout << "Kamu melewati GO.\n";
    std::cout << "Kamu mendapatkan M" << gameManager.getConfig().goSalary << '\n';
}

