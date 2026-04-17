#include "include/views/GameView.hpp"
#include <sstream>
void GameView::InputNextCommand(){
    std::cout << "> ";
    std::string input;
    std::getline(std::cin, input);
    
    std::stringstream ss(input);
    std::string command;
    ss >> command;
    if(command == "CETAK_PAPAN"){
        boardView.drawBoard();
    }
    if(command == "LEMPAR_DADU"){
        diceView.outputRollDice();
        gameManager.processRollDice();
        diceView.outputRolledDice(DiceRoller::getLastRoll().first, DiceRoller::getLastRoll().second);
    }
    if(command == "ATUR_DADU"){
        int val1,val2;
        ss >> val1 >> val2;
        gameManager.processSetDice(val1,val2);
        diceView.outputSetDice(val1,val2);
    }
    if(command == "CETAK_AKTA"){
        propertyView.outputRequestTileCode();
        std::string tileCode;
        cin >> tileCode;
        int pos = gameManager.getBoard().geTilePosition(tileCode);
        if(pos == -1){
            propertyView.outputPropertyTileNotFound();
        }
        else{
            Tile& tile = gameManager.getBoard().getTile(pos);
            Property& prop = dynamic_cast<Property&>(tile);
            propertyView.outputProperty(prop);
        }
        
    }
    if(command == "CETAK_PROPERTI"){
        propertyView.outputPlayerProperties(gameManager.getCurrentPlayer());
    }
    if(command == "GADAI"){
        gameManager.processMortgageProperty();
    }
    if(command == "TEBUS"){
        gameManager.processUnmortgageProperty();
    }
    if(command == "BANGUN"){
        gameManager.processBuild();
    }
    if(command == "SIMPAN"){
        std::string pathFile;
        ss >> pathFile;
        gameManager.processSaveGame(pathFile);
    }
    if(command == "MUAT"){
        std::string pathFile;
        ss >> pathFile;
        gameManager.processLoadGame(pathFile);
    }
    if(command == "GUNAKAN_KEMAMPUAN"){
        gameManager.processUseSkillCard();
    }
    if(command == "CETAK_LOG"){
        int logAmount;
        if(ss >> logAmount){
            std::cout << "=== Log Transaksi (" << logAmount << " Terakhir) ===";
            logView.printLogView(logAmount);
        }
        else{
            std::cout << "=== Log Transaksi Penuh ===\n";
            logView.printLogView();
        }
    }
}