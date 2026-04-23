#pragma once
#include "IGuiState.hpp"
#include "GuiConfig.hpp"

#include <sstream>
#include <algorithm>
#include <ctime>

class GuiState : public IGuiState {
public:
    GuiState() {
        buildDefaultBoard();
        buildPlayers();
        GUIlines.push_back("Test");
    }

    const std::vector<TileInfo>& tiles() const override { return GUItiles; }
    const std::vector<PlayerInfo>& players()  const override { return GUIplayers; }
    int  currentPlayerIdx() const override { return GUIcurPlayer; }
    int  currentTurn() const override { return GUIturn; }
    int  maxTurn()const override { return 30; }
    DiceInfo dice() const override { return GUIdice; }
    const std::vector<std::string>& consoleLines() const override { return GUIlines; }

    void update(float dt) {
        if (GUIdice.rolling) {
            GUIdice.rollTimer += dt;
            if (GUIdice.rollTimer >= GuiConfig::DICE_SPIN_DUR) {
                GUIdice.rolling   = false;
                GUIdice.rollTimer = 0.f;
            }
        }
    }

    void submitCommand(const std::string& raw) override {
        std::istringstream ss(raw);
        std::string cmd; ss >> cmd;
        for (auto& c : cmd) c = (char)toupper(c);
    }
    
    bool hasPendingMove(int& outPlayer, int& outSteps) {
        if (GUIpendingMove.second <= 0) return false;
        outPlayer = GUIpendingMove.first;
        outSteps  = GUIpendingMove.second;
        GUIpendingMove = {0,0};
        return true;
    }

    void setPlayerPos(int idx, int pos) {
        if (idx >= 0 && idx < 4) GUIplayers[idx].pos = pos;
    }

private:
    void buildDefaultBoard() {
        // 40 tiles matching the spec layout
        struct T { int pos; const char* code; const char* name;
                   const char* cg; const char* type; };
        const T spec[] = {
            { 1,"GO",  "Mulai",           "DEFAULT","GO"},
            { 2,"GRT", "Garut",           "COKLAT","STREET"},
            { 3,"DNU", "Dana Umum",       "DEFAULT","COMMUNITY"},
            { 4,"TSK", "Tasikmalaya",     "COKLAT","STREET"},
            { 5,"PPH", "Pajak Pghsln",   "DEFAULT","INCOME_TAX"},
            { 6,"GBR", "Sta.Gambir",      "DEFAULT","RAILROAD"},
            { 7,"BGR", "Bogor",           "BIRU_MUDA","STREET"},
            { 8,"FES", "Festival",        "DEFAULT","FESTIVAL"},
            { 9,"DPK", "Depok",           "BIRU_MUDA","STREET"},
            {10,"BKS", "Bekasi",          "BIRU_MUDA","STREET"},
            {11,"PEN", "Penjara",         "DEFAULT","JAIL"},
            {12,"MGL", "Magelang",        "MERAH_MUDA","STREET"},
            {13,"PLN", "PLN",             "ABU_ABU","UTILITY"},
            {14,"SOL", "Solo",            "MERAH_MUDA","STREET"},
            {15,"YOG", "Yogyakarta",      "MERAH_MUDA","STREET"},
            {16,"STB", "Sta.Bandung",     "DEFAULT","RAILROAD"},
            {17,"MAL", "Malang",          "ORANGE","STREET"},
            {18,"DNU", "Dana Umum",       "DEFAULT","COMMUNITY"},
            {19,"SMG", "Semarang",        "ORANGE","STREET"},
            {20,"SBY", "Surabaya",        "ORANGE","STREET"},
            {21,"BBP", "Bebas Parkir",    "DEFAULT","FREE_PARKING"},
            {22,"MKS", "Makassar",        "MERAH","STREET"},
            {23,"KSP", "Kesempatan",      "DEFAULT","CHANCE"},
            {24,"BLP", "Balikpapan",      "MERAH","STREET"},
            {25,"MND", "Manado",          "MERAH","STREET"},
            {26,"TUG", "Sta.Tugu",        "DEFAULT","RAILROAD"},
            {27,"PLB", "Palembang",       "KUNING","STREET"},
            {28,"PKB", "Pekanbaru",       "KUNING","STREET"},
            {29,"PAM", "PAM",             "ABU_ABU","UTILITY"},
            {30,"MED", "Medan",           "KUNING","STREET"},
            {31,"PPJ", "Pergi ke Pnjr",  "DEFAULT","GO_TO_JAIL"},
            {32,"BDG", "Bandung",         "HIJAU","STREET"},
            {33,"DEN", "Denpasar",        "HIJAU","STREET"},
            {34,"FES", "Festival",        "DEFAULT","FESTIVAL"},
            {35,"MTR", "Mataram",         "HIJAU","STREET"},
            {36,"GUB", "Sta.Gubeng",      "DEFAULT","RAILROAD"},
            {37,"KSP", "Kesempatan",      "DEFAULT","CHANCE"},
            {38,"JKT", "Jakarta",         "BIRU_TUA","STREET"},
            {39,"PBM", "Pajak Brg Mwh",  "DEFAULT","LUXURY_TAX"},
            {40,"IKN", "Ibu Kota Nus.",   "BIRU_TUA","STREET"},
        };
        for (auto& s : spec) {
            TileInfo t{};
            t.pos = s.pos;
            t.code = s.code;
            t.name = s.name;
            t.colorGroup = s.cg;
            t.tileType = s.type;
            t.ownerIdx = -1;
            t.buildings = 0;
            t.mortgaged = false;
            t.festivalActive= false;
            t.festivalMult = 1;
            GUItiles.push_back(t);
        }
    }

    void setOwner(const std::string& code, int idx) {
        for (auto& t : GUItiles)
            if (t.code == code) {
                t.ownerIdx  = idx;
                t.ownerName = "P" + std::to_string(idx+1);
                break;
            }
    }

    void buildPlayers() {
        const char* names[] = {"player1","player2","player3","player4"};
        const int   poss[]  = {1, 1, 1, 1};
        for (int i = 0; i < 4; ++i) {
            PlayerInfo p{};
            p.idx = i;
            p.username = names[i];
            p.pos = poss[i];
            p.money = 1500 - i*200;
            p.state = "ACTIVE";
            p.isCurrent= (i == 0);
            GUIplayers.push_back(p);
        }
    }

    std::vector<TileInfo>  GUItiles;
    std::vector<PlayerInfo>  GUIplayers;
    std::vector<std::string> GUIlines;
    DiceInfo GUIdice;
    int GUIcurPlayer = 0;
    int GUIturn  = 1;
    std::pair<int,int> GUIpendingMove{0,0};
};
