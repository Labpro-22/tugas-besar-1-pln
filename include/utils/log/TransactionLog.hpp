#pragma once

#include <string>

class TransactionLog {
private:
    int turn;
    std::string username;
    std::string action;
    std::string details;

public:
    TransactionLog(int turn, std::string username, std::string action, std::string details);

    int getTurn() const;
    std::string getUsername() const;
    std::string getAction() const;
    std::string getDetails() const;

    std::string toString() const;
};

/*
Lemparan dadu dan pergerakan bidak
Pembelian properti (termasuk perolehan Railroad/Utility otomatis)
Pembayaran sewa dan pajak
Pembangunan dan penjualan bangunan
Gadai dan tebus properti
Penggunaan kartu kemampuan spesial
Pengambilan kartu Kesempatan dan Dana Umum
Setiap bid dan hasil akhir lelang
Aktivasi dan penguatan efek festival
Kebangkrutan dan pengambilalihan aset
Save dan load permainan
*/