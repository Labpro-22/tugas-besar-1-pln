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
        for (const auto& entry : std::filesystem::directory_iterator("data")) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();

                if (entry.path().extension() == ".txt") {
                    std::string nameOnly = entry.path().stem().string();
                    std::cout << "- " << nameOnly << "\n";
                }
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cout << "Error membuka folder: " << e.what() << "\n";
    }
    std::cout << "\n";
}

std::string LoadView::promptSaveName(){
    std::cout << "Masukkan nama file: ";
    std::string fileName;
    std::cin >> fileName;
    std::cout << "\n\n";
    return fileName;
}