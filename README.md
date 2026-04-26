# Nimonspoli

Tugas Besar 1 — IF2010 Pemrograman Berorientasi Objek

Nimonspoli adalah permainan papan berbasis teks yang terinspirasi dari Monopoli. Dua hingga empat pemain bersaing untuk membangun kekayaan dengan membeli properti, memungut sewa, mengikuti lelang, dan mengelola aset — sambil menghadapi kartu kejutan, efek festival, dan risiko kebangkrutan. Permainan ini dibangun sepenuhnya dalam C++ dengan penerapan konsep-konsep OOP.

---

## Cara Menjalankan

Pastikan `g++` dengan dukungan C++17 dan `make` sudah terpasang di sistem.

```bash
# Compilation
make

# Running
make run

# Cleaning
make clean

# Rebuild dari awal
make rebuild
```

File konfigurasi permainan dibaca dari direktori `config/`. Pastikan direktori tersebut tersedia sebelum menjalankan program.

---

## Struktur Proyek

```
.
├── config/                        # File konfigurasi permainan
│   ├── property.txt
│   ├── railroad.txt
│   ├── utility.txt
│   ├── tax.txt
│   ├── special.txt
│   ├── misc.txt
│   └── aksi.txt
├── include/
│   ├── core/                      # Logic layer: aturan dan state permainan
│   │   ├── GameManager.hpp
│   │   ├── Nimonspoli.hpp
│   │   └── DiceRoller.hpp
│   ├── models/                    # Entitas permainan
│   │   ├── Bank.hpp
│   │   ├── Player.hpp
│   │   ├── card/
│   │   │   ├── CardDeck.hpp
│   │   │   ├── chance-card/
│   │   │   ├── community-chest-card/
│   │   │   └── skill-card/
│   ├── utils/
│   │   ├── config/                # Konfigurasi (data layer)
│   │   ├── save-load/             # Save & load state (data layer)
│   └── views/                     # UI layer: output ke terminal
├── src/                           # Implementasi (.cpp)
├── data/                          # Direktori save file (runtime)
├── makefile
└── README.md
```

---

## Daftar Perintah

Perintah dimasukkan langsung di terminal selama giliran pemain.

| Perintah | Keterangan |
|---|---|
| `CETAK_PAPAN` | Tampilkan papan permainan beserta posisi semua pemain |
| `LEMPAR_DADU` | Lempar dadu secara acak |
| `ATUR_DADU X Y` | Tentukan hasil dadu secara manual (nilai 1–6) |
| `CETAK_AKTA [KODE]` | Tampilkan akta kepemilikan sebuah properti |
| `CETAK_PROPERTI` | Tampilkan daftar properti milik pemain aktif |
| `GADAI` | Gadaikan properti ke bank |
| `TEBUS` | Tebus properti yang sedang digadaikan |
| `BANGUN` | Bangun rumah atau hotel di atas properti |
| `GUNAKAN_KEMAMPUAN` | Aktifkan kartu kemampuan spesial dari tangan |
| `BAYAR_DENDA` | Bayar denda untuk keluar dari penjara |
| `SIMPAN [NAMA_FILE]` | Simpan state permainan ke file |
| `MUAT [NAMA_FILE]` | Muat permainan dari file yang tersimpan |
| `CETAK_LOG [N]` | Tampilkan N entri log terakhir; tanpa argumen menampilkan semua |
| `SELESAI` | Akhiri giliran saat ini |
| `BANTUAN` | Tampilkan daftar perintah yang tersedia |

---

## Arsitektur

Program mengikuti **layered architecture** tiga lapis untuk memisahkan tanggung jawab antar komponen.

**UI Layer** (`include/views/`, `src/views/`) — Menangani seluruh input dari pengguna dan output ke terminal. Tidak mengandung logika permainan.

**Core Layer** (`include/core/`, `include/models/`, `src/core/`, `src/models/`) — Inti permainan: aturan, kalkulasi sewa dan pajak, manajemen state, pergerakan bidak, dan alur turn. Semua keputusan permainan ditentukan di sini.

**Data Layer** (`include/utils/`, `src/utils/`) — Membaca file konfigurasi saat startup dan menangani operasi save/load ke file teks.

---

## Konsep OOP

**Inheritance dan Polymorphism** — `Tile` menjadi kelas dasar untuk seluruh jenis petak (`PropertyTile`, `StartTile`, `JailTile`, `FestiveTile`, dll.). `Card` diturunkan ke `ChanceCard`, `CommunityChestCard`, dan `SkillCard` yang masing-masing memiliki subclass tersendiri. `Property` diturunkan ke `StreetProperty`, `RailroadProperty`, dan `UtilityProperty`.

**Exception Handling** — Validasi uang tidak cukup saat membeli properti atau membayar sewa, validasi batas maksimal kartu di tangan, serta penanganan kesalahan baca/tulis file save.

**Operator Overloading** — Operator `+=` dan `-=` pada `Player` untuk transaksi uang. Operator `<` dan `>` untuk membandingkan total kekayaan antar pemain saat menentukan pemenang.

**Abstract Class dan Virtual Function** — `Tile` mendefinisikan `onLanded()` dan `onPassBy()` sebagai virtual function yang diimplementasikan berbeda oleh tiap jenis petak. `Property` mendefinisikan `calculateRent()` yang di-override oleh masing-masing jenis properti.

**Generic Class** — `CardDeck<T>` adalah template class yang digunakan untuk mengelola tumpukan kartu Kesempatan, Dana Umum, dan Kartu Kemampuan Spesial.

**STL** — `std::vector` untuk menyimpan daftar pemain, petak, dan kartu. `std::map` untuk tabel sewa berdasarkan jumlah properti (Railroad, Utility) dan untuk mencari petak berdasarkan kode.

---

## Kartu Kemampuan Spesial

Setiap awal giliran, pemain menerima satu kartu acak. Maksimal tiga kartu dapat dipegang sekaligus; jika mendapat kartu keempat, pemain wajib membuang satu. Kartu hanya bisa digunakan sebelum melempar dadu, dan hanya satu kartu per giliran.

| Kartu | Jumlah | Efek |
|---|---|---|
| MoveCard | 4 | Maju sejumlah petak yang ditentukan saat kartu diterima |
| DiscountCard | 3 | Diskon persentase acak untuk satu giliran |
| ShieldCard | 2 | Kebal dari tagihan sewa dan sanksi selama satu giliran |
| TeleportCard | 2 | Pindah ke petak mana saja di papan |
| LassoCard | 2 | Tarik satu pemain lawan ke petak pemain saat ini |
| DemolitionCard | 2 | Hancurkan satu bangunan milik pemain lawan |

---

## Anggota Kelompok

| NIM | Nama |
|---|---|
| 13524016 | Wafiq Hibban Robbany |
| 13524018 | Muhammad Nafis Habibi |
| 13524038 | An-Dafa Anza Avansyah|
| 13524040 |  Kloce Paul William Saragih |
| 13524054 | Wildan Abdurrahman Ghazali |
| 13524108 | Daffa Mutaqin Tetaputra |
