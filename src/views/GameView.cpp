#include "core/GameManager.hpp"
#include "views/GameView.hpp"
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
        gameManager.processRollDice();
    }
    if(command == "ATUR_DADU"){
        int val1,val2;
        ss >> val1 >> val2;
        gameManager.processSetDice(val1,val2);
    }
    if(command == "CETAK_AKTA"){
        propertyView.outputProperty();
    }
    if(command == "CETAK_PROPERTI"){
        propertyView.outputPlayerProperties();
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
            logView.printLogView(logAmount);
        }
        else{
            logView.printLogView();
        }
    }
}