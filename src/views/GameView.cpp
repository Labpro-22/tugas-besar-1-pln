#include "core/GameManager.hpp"
#include "views/GameView.hpp"
#include <sstream>
void GameView::inputNextCommand(){
    std::string input;

    
    std::cout << gameManager.getCurrentPlayer().getUsername() << "> ";
    std::cin >> std::ws;
    std::getline(std::cin, input);
    std::stringstream ss(input); 
    std::string command;
    ss >> command;
    bool handled = true;
    if(command == "CETAK_PAPAN"){
        boardView.drawBoard();
    }
    else if(command == "LEMPAR_DADU"){
        gameManager.processRollDice();
    }
    else if(command == "ATUR_DADU"){
        int val1,val2;
        if (ss >> val1 >> val2) {
            gameManager.processSetDice(val1,val2);
        }
        else {
            std::cout << "Format ATUR_DADU tidak valid. Gunakan: ATUR_DADU <dadu1> <dadu2>\n\n";
        }
    }
    else if(command == "CETAK_AKTA"){
        propertyView.outputProperty();
    }
    else if(command == "CETAK_PROPERTI"){
        propertyView.outputPlayerProperties();
    }
    else if(command == "CETAK_INFO"){
        mainMenuView.outputCurrentPlayerInfo();
    }
    else if(command == "GADAI"){
        gameManager.processMortgageProperty();
    }
    else if(command == "TEBUS"){
        gameManager.processUnmortgageProperty();
    }
    else if(command == "BANGUN"){
        gameManager.processBuild();
    }
    else if(command == "SIMPAN"){
        std::string pathFile;
        if (ss >> pathFile) {
            gameManager.processSaveGame(pathFile);
        }
        else {
            std::cout << "Format SIMPAN tidak valid. Gunakan: SIMPAN <nama_file>\n\n";
        }
    }
    else if(command == "GUNAKAN_KEMAMPUAN"){
        gameManager.processUseSkillCard();
    }
    else if(command == "CETAK_LOG"){
        int logAmount;
        if(ss >> logAmount){
            logView.printLogView(logAmount);
        }
        else{
            logView.printLogView();
        }
    }
    else if (command == "HELP") {
        std::cout << "\n=== DAFTAR PERINTAH YANG TERSEDIA ===\n\n";
        std::cout << "CETAK_PAPAN" << "                 - Menampilkan papan permainan\n";
        std::cout << "LEMPAR_DADU" << "                 - Melempar dadu dan bergerak\n";
        std::cout << "ATUR_DADU <dadu1> <dadu2>" << "   - Mengatur hasil dadu secara manual\n";
        std::cout << "CETAK_AKTA" << "                  - Menampilkan akta properti\n";
        std::cout << "CETAK_PROPERTI" << "              - Menampilkan properti yang dimiliki\n";
        std::cout << "CETAK_INFO" << "                  - Menampilkan informasi pemain saat ini\n";
        std::cout << "GADAI" << "                       - Menggadaikan properti\n";
        std::cout << "TEBUS" << "                       - Menebus properti yang digadaikan\n";
        std::cout << "BANGUN" << "                      - Membangun rumah/hotel di properti\n";
        std::cout << "SIMPAN <nama_file>" << "          - Menyimpan permainan ke file\n";
        std::cout << "GUNAKAN_KEMAMPUAN" << "           - Menggunakan kartu kemampuan\n";
        std::cout << "CETAK_LOG [jumlah]" << "          - Menampilkan log transaksi (opsional: jumlah baris)\n";
        std::cout << "HELP" << "                        - Menampilkan daftar perintah\n";
        std::cout << std::endl;
    }
    else {
        handled = false;
    }

    if (!handled && !command.empty()) {
        std::cout << "Perintah tidak dikenali: " << command << "\n\n";
    }
}
