#include "core/GameManager.hpp"
#include "views/LoadView.hpp"
#include <filesystem>

void LoadView::outputLoading()const{
    std::cout << "Memuat permainan";
    for (int i = 0; i < 3; i++) {
        std::cout << ".";
        std::cout.flush(); 
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::cout <<"\n\n";
}

void LoadView::outputLoadStatus(bool success)const{
    if(success){
            std::cout << "Permainan berhasil dimuat. Melanjutkan giliran " << gameManager.getCurrentPlayer().getUsername() ;
            for (int i = 0; i < 3; i++) {
                std::cout << ".";
                std::cout.flush(); 
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
            std::cout << "\n";
            }
    else std::cout<< "Gagal memuat file! File rusak atau format tidak dikenali.\n";
    
}



void LoadView::outputSaveNames() const{
    try {
        bool first = true;
        for (const auto& entry : std::filesystem::directory_iterator("data")) {
            if (entry.is_regular_file()) {
                if (first) {
                    std::cout << "Nama save file:\n";
                    first = false;
                }
                std::string filename = entry.path().filename().string();
                std::cout << "- " << filename << "\n";
                // if (entry.path().extension() == ".txt") {
                //     std::string nameOnly = entry.path().stem().string();
                // }
            }
        }
        if (first) {
            std::cout << "Belum ada permainan yang disimpan\n";
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cout << "Error membuka folder: " << e.what() << "\n";
    }
    std::cout << std::endl;
}

std::string LoadView::promptSaveName(){
    std::cout << "Masukkan nama file: ";
    std::string fileName;
    std::cin >> fileName;
    std::cout << "\n\n";
    return fileName;
}