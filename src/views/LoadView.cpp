#include "core/GameManager.hpp"
#include "views/LoadView.hpp"

void LoadView::outputLoading()const{
    std::cout << "Memuat permainan";
    for (int i = 0; i < 3; i++) {
        std::cout << ".";
        std::cout.flush(); 
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::cout <<"\n\n";
}

void LoadView::outputLoadStatus(std::string pathFile, bool fileFound, bool fileNotCorrupted)const{
    if(fileFound){
        if(fileNotCorrupted){
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
    else std::cout << "File \"" << pathFile << " \" tidak ditemukan.\n";
}