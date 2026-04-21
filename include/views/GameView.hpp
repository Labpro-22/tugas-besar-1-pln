#pragma once
#include <iostream>

#include "AuctionView.hpp"
#include "BankruptView.hpp"
#include "BoardView.hpp"
#include "BuildView.hpp"
#include "BuyView.hpp"
#include "CardView.hpp"
#include "DiceView.hpp"
#include "DropSkillCardView.hpp"
#include "FestivalView.hpp"
#include "LoadView.hpp"
#include "MainMenuView.hpp"
#include "MortgageView.hpp"
#include "PropertyView.hpp"
#include "SaveView.hpp"
#include "UnmortgageView.hpp"
#include "UseSkillCardView.hpp"
#include "WinView.hpp"
#include "LogView.hpp"



class GameView
{
private:
    GameManager &gameManager;

    AuctionView auctionView;
    BankruptView bankruptView;
    BoardView boardView;
    BuildView buildView;
    BuyView buyView;
    CardView cardView;
    DiceView diceView;
    DropSkillCardView dropSkillCardView;
    FestivalView festivalView;
    LoadView loadView;
    LogView logView;
    MainMenuView mainMenuView;
    MortgageView mortgageView;
    PropertyView propertyView;
    SaveView saveView;
    UnmortgageView unmortgageView;
    UseSkillCardView useSkillCardView;
    WinView winView;

public:
GameView(GameManager &gm)
        : gameManager(gm),
          auctionView(gm),
          bankruptView(gm),
          boardView(gm),
          buildView(gm),
          buyView(gm),
          cardView(gm),
          diceView(gm),
          dropSkillCardView(gm),
          festivalView(gm),
          loadView(gm),
          logView(gm),
          mainMenuView(gm),
          mortgageView(gm),
          propertyView(gm),
          saveView(gm),
          unmortgageView(gm),
          useSkillCardView(gm),
          winView(gm)
    {
    }
    AuctionView& getAuctionView(){
        return auctionView;
    }

    BankruptView& getBankruptView() {
        return bankruptView;
    }

    BoardView& getBoardView() {
        return boardView;
    }

    BuildView& getBuildView() {
        return buildView;
    }

    BuyView& getBuyView() {
        return buyView;
    }

    CardView& getCardView() {
        return cardView;
    }

    
    DiceView& getDiceView() {
        return diceView;
    }

    DropSkillCardView& getDropSkillCardView() {
        return dropSkillCardView;
    }

    FestivalView& getFestivalView() {
        return festivalView;
    }

    LoadView& getLoadView() {
        return loadView;
    }

    MainMenuView& getMainMenuView() {
        return mainMenuView;
    }

    MortgageView& getMortgageView() {
        return mortgageView;
    }

    PropertyView& getPropertyView() {
        return propertyView;
    }

    SaveView& getSaveView() {
        return saveView;
    }

    UnmortgageView& getUnmortgageView() {
        return unmortgageView;
    }

    UseSkillCardView& getUseSkillCardView() {
        return useSkillCardView;
    }

    WinView& getWinView() {
        return winView;
    }

    LogView& getLogView() {
        return logView;
    }
    void InputNextCommand();
};
