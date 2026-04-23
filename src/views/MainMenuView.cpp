#include "core/GameManager.hpp"
#include "views/MainMenuView.hpp"

int MainMenuView::promptNewGameOrLoadGame(){
    std::cout << "\033[2J\033[1;1H";




    std::cout << BOLD << CYAN;
    std::cout << "=====================================\n";
    std::cout << "        N I M O N S P O L I\n";
    std::cout << "=====================================\n";
    std::cout << RESET;


    int input;
    while (true)
    {
        std::cout << GREEN << "1. New Game\n" << RESET;
        std::cout << YELLOW << "2. Load Game\n" << RESET;
        std::cout << RED << "0. Exit\n" << RESET;

        std::cout << "\n";
        std::cout << BOLD << "Pilih opsi: " << RESET;
        std::cin >> input;
        if(input >= 0 || input <= 2){
            return input;
        }
        std::cout << RED << "Masukan tidak valid!\n\n" << RESET;
    }
}

std::vector<std::string> MainMenuView::promptUsernames(){
    int n;
    std::string line;
    while (true) {
        std::cout << CYAN << "Masukkan jumlah pemain (2-4): " << RESET;
        // std::getline(std::cin, line);
        // if (line.empty()) {
        //     std::cout << RED << "Input tidak boleh kosong!\n" <<RESET;
        //     continue;
        // }
        // std::stringstream ss(line);
        if (!(std::cin >> n) || n < 2 || n > 4) {
            std::cout << RED << "Input tidak valid! Harus antara 2 sampai 4.\n" << RESET;
            continue;
        }
        else {
            break;
        }
    }
    std::vector<std::string> names;
    std::string name;

    for (int i = 0; i < n; i++) {
        while (true) {
            std::cout << CYAN << "Masukkan username pemain " << (i + 1) << " (Maks. panjang nama = 8 karakter): " << RESET;
            std::getline(std::cin, name);

            if (name.empty()) {
                std::cout << RED << "Username tidak boleh kosong!\n" << RESET;
                continue;
            }

            if (std::find(names.begin(), names.end(), name) != names.end()) {
                std::cout << RED << "Username sudah digunakan! Pilih nama lain.\n" << RESET;
                continue;
            }
            if(name.length() > 8){
                std::cout <<RED<<"Panjang username lebih dari 8! Pilih nama lain!\n" << RESET;
                continue;
            }
            names.push_back(name);
            break;
        }
    }
    std::cout << "\n" << BOLD << YELLOW << "Daftar Pemain:\n" << RESET;
    for (int i = 0; i < (int) names.size(); i++) {
        std::cout << GREEN << i + 1 << ". " << names[i] << RESET << "\n";
    }
    return names;
}

void MainMenuView::outputCurrentPlayer() {
    std::cout << "Giliran pemain " << gameManager.getCurrentPlayer().getUsername() << "\n";
}
