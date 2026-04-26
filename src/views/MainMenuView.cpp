#include "core/GameManager.hpp"
#include "views/MainMenuView.hpp"
#include <sstream>

int MainMenuView::promptNewGameOrLoadGame(){




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
        if(input >= 0 && input <= 2){
            return input;
        }
        std::cout << RED << "Masukan tidak valid!\n\n" << RESET;
    }
}

std::vector<std::string> MainMenuView::promptUsernames(){
    int n;
    std::string line;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while (true) {
        do {
            std::cout << CYAN << "Masukkan jumlah pemain (2-4): " << RESET;
            std::getline(std::cin, line);
        } while (line.empty());
        std::stringstream ss(line);
        if (!(ss >> n) || !(ss.eof()) || n < 2 || n > 4) {
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
    std::cout << YELLOW << "\nGiliran pemain " << gameManager.getCurrentPlayer().getUsername() << "\n";
}

void MainMenuView::outputCurrentPlayerInfo() {
    Player &player = gameManager.getCurrentPlayer();
    Tile *tile = player.getPiece().getCurrentTile();

    std::cout << YELLOW << "\nGiliran pemain " << player.getUsername() << "\n" << RESET;
    std::cout << "Status   : "
              << (player.getState() == PlayerState::ACTIVE ? "ACTIVE"
                  : player.getState() == PlayerState::JAILED ? "JAILED"
                                                             : "BANKRUPT")
              << "\n";
    if (tile != nullptr) {
        std::cout << "Posisi   : " << tile->getName() << " [" << tile->getCode() << "]\n";
    }
    std::cout << "Uang     : M" << player.getMoney() << "\n";
    std::cout << "Properti : " << player.getProperties().size() << "\n";

    std::ostringstream skillInfo;
    const auto &cards = player.getSkillCards();
    if (cards.empty()) {
        skillInfo << "-";
    }
    else {
        for (size_t i = 0; i < cards.size(); ++i) {
            if (i > 0) {
                skillInfo << ", ";
            }
            skillInfo << cards[i]->getCardType();
        }
    }
    std::cout << "Skill    : " << skillInfo.str() << "\n";

    std::ostringstream effectInfo;
    const auto &effects = player.getEffects();
    if (effects.empty()) {
        effectInfo << "-";
    }
    else {
        for (size_t i = 0; i < effects.size(); ++i) {
            if (i > 0) {
                effectInfo << ", ";
            }
            effectInfo << effects[i].getName();
            if (effects[i].getValue() != 0) {
                effectInfo << "(" << effects[i].getValue() << ")";
            }
            if (!effects[i].getIsOneTimeUse()) {
                effectInfo << "[" << effects[i].getDuration() << "]";
            }
        }
    }
    std::cout << "Efek     : " << effectInfo.str() << "\n";

    if (player.isJailed()) {
        std::cout << "Penjara  : " << player.getJailTurns() << " giliran\n";
    }

    std::cout << std::endl;
}
