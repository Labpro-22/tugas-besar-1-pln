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
    const AuctionView& getAuctionView() const{
        return auctionView;
    }

    const BankruptView& getBankruptView() const {
        return bankruptView;
    }

    const BoardView& getBoardView() const {
        return boardView;
    }

    const BuildView& getBuildView() const {
        return buildView;
    }

    const BuyView& getBuyView() const {
        return buyView;
    }

    const CardView& getCardView() const {
        return cardView;
    }

    
    const DiceView& getDiceView() const {
        return diceView;
    }

    const DropSkillCardView& getDropSkillCardView() const {
        return dropSkillCardView;
    }

    const FestivalView& getFestivalView() const {
        return festivalView;
    }

    const LoadView& getLoadView() const {
        return loadView;
    }

    const MainMenuView& getMainMenuView() const {
        return mainMenuView;
    }

    const MortgageView& getMortgageView() const {
        return mortgageView;
    }

    const PropertyView& getPropertyView() const {
        return propertyView;
    }

    const SaveView& getSaveView() const {
        return saveView;
    }

    const UnmortgageView& getUnmortgageView() const {
        return unmortgageView;
    }

    const UseSkillCardView& getUseSkillCardView() const {
        return useSkillCardView;
    }

    const WinView& getWinView() const {
        return winView;
    }

    const LogView& getLogView() const{
        return logView;
    }
    void InputNextCommand();
};
