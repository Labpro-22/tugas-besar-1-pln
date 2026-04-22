#include "core/GameManager.hpp"
#include "LogView.hpp"

void LogView::printLogView()const{
    std::cout << "=== Log Transaksi Penuh ===\n\n";
    TransactionLogger logger = gameManager.getLogger();
    for (TransactionLog log : logger.getLogs()) {
        std::cout << log.toString() << "\n";
    }
}
void LogView::printLogView(int logAmount)const{
    std::cout << "=== Log Transaksi (" << logAmount << " Terakhir) ===\n\n";
    TransactionLogger logger = gameManager.getLogger();
    int i = 1;
    for(TransactionLog log : logger.getLogs()){
        if(i >= logAmount)return;
        std::cout << log.toString() << "\n";
    }
}