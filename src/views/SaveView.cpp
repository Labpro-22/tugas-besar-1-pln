#include "core/GameManager.hpp"
#include "views/SaveView.hpp"

void SaveView::outputSaving() const{
    std::cout << "Menyimpan permainan";
    for (int i = 0; i < 3; i++) {
        std::cout << ".";
        std::cout.flush(); 
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::cout <<"\n\n";
}
int SaveView::promptOverwriteSaveFile(std::string path) const{
    std::cout << "File \"" << path << "\" sudah ada.\nTimpa file lama? (y/n): " ;
    std::string yayOrNay;
    while(true){
        std::cin >> yayOrNay;
        if(yayOrNay == "y"){
            return 1;
        }
        if(yayOrNay == "n"){
            return 0;
        }
        std::cout << "Input tidak valid! File \"" << path << "\" sudah ada.\nTimpa file lama? (y/n): " ;
    }
    
}
void SaveView::outputSaveStatus(bool success, std::string path) const{
    if(success){
        std::cout << "Permainan berhasil disimpan ke: " << path << "\n\n";
    }
    else{
        std::cout << "Gagal menyimpan file! Pastikan direktori dapat ditulis.\n\n";
    }
}