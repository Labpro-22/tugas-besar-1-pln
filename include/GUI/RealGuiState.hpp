#pragma once

#include "GUI/IGuiState.hpp"
#include "GUI/GuiConfig.hpp"  
#include "core/GameManager.hpp"
#include "models/tile/PropertyTile.hpp"
#include "models/property/StreetProperty.hpp"
#include "utils/DiceRoller.hpp"

#include <sstream>
#include <streambuf>
#include <mutex>
#include <deque>
#include <atomic>
#include <thread>
#include <chrono>

// Stream buffer that captures cout into a thread-safe line queue
class GuiStreamBuf : public std::streambuf {
public:
    void getLines(std::vector<std::string>& out) const {
        std::lock_guard<std::mutex> lk(mx);
        out.assign(lines.begin(), lines.end());
    }

protected:
    int overflow(int c) override {
        if (c == EOF) return c;
        std::lock_guard<std::mutex> lk(mx);
        current += (char)c;
        if (c == '\n') {
            lines.push_back(current);
            current.clear();
            if (lines.size() > 500) lines.pop_front();
        }
        return c;
    }

    int sync() override { return 0; }

private:
    mutable std::mutex mx;    
    mutable std::deque<std::string> lines;
    mutable std::string current;
};

// Stream buffer that feeds GUI-submitted commands into cin
class GuiInputBuf : public std::streambuf {
public:
    void push(const std::string& cmd) {
        std::lock_guard<std::mutex> lk(mx);
        for (char c : cmd) buf.push_back(c);
        buf.push_back('\n');
    }

    bool hasData() {
        std::lock_guard<std::mutex> lk(mx);
        return !buf.empty();
    }

protected:
    int underflow() override {
        while (true) {
            {
                std::lock_guard<std::mutex> lk(mx);
                if (!buf.empty()) {
                    ch = buf.front();
                    buf.pop_front();
                    setg(&ch, &ch, &ch + 1);
                    return (unsigned char)ch;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }

private:
    std::mutex mx;
    std::deque<char> buf;
    char ch = 0;
};

class RealGuiState : public IGuiState {
public:
    explicit RealGuiState(GameManager& gm) : gm(gm) {
        oldCoutBuf = std::cout.rdbuf(&coutBuf);
        oldCinBuf  = std::cin.rdbuf(&cinBuf);
    }

    ~RealGuiState() {
        std::cout.rdbuf(oldCoutBuf);
        std::cin.rdbuf(oldCinBuf);
    }

    const std::vector<TileInfo>& tiles() const override {
        refreshTiles();
        return cachedTiles;
    }

    const std::vector<PlayerInfo>& players() const override {
        refreshPlayers();
        return cachedPlayers;
    }

    int currentPlayerIdx() const override {
        try {
            const auto& all = gm.getPlayers();
            const Player& cur = gm.getCurrentPlayer();
            for (int i = 0; i < (int)all.size(); ++i) {
                if (&all[i] == &cur) return i;
            }
        } catch (...) {}
        return 0;
    }

    int currentTurn() const override { return gm.getCurrentTurn(); }
    int maxTurn()     const override { return gm.getConfig().maxTurn; }

    DiceInfo dice() const override {
        DiceInfo d;
        auto roll = DiceRoller::getLastRoll();
        d.d1 = roll.first;
        d.d2 = roll.second;
        d.rolling   = diceRolling.load();
        d.rollTimer = diceTimer.load();
        return d;
    }

    const std::vector<std::string>& consoleLines() const override {
        coutBuf.getLines(lineCache);
        return lineCache;
    }

    void submitCommand(const std::string& cmd) override {
        cinBuf.push(cmd);
        std::string echo;
        try {
            echo = gm.getCurrentPlayer().getUsername() + "> " + cmd + "\n";
        } catch (...) {
            echo = "> " + cmd + "\n";
        }
        for (char c : echo) coutBuf.sputc(c);
    }

    void updateDiceTimer(float dt) {
        if (diceRolling.load()) {
            float t = diceTimer.load() + dt;
            diceTimer.store(t);
            if (t >= GuiConfig::DICE_SPIN_DUR) {
                diceRolling.store(false);
                diceTimer.store(0.f);
            }
        }
    }

    void triggerDiceAnim() {
        diceRolling.store(true);
        diceTimer.store(0.f);
    }

    bool isDiceRolling() const { return diceRolling.load(); }

    void notifyRoll(int playerIdx, int fromPos, int steps) override {
        std::lock_guard<std::mutex> lk(moveMx);
        pendingMove = {playerIdx, fromPos, steps};
        hasPending  = true;
        diceRolling.store(true);
        diceTimer.store(0.f);
    }

    bool takePendingMove(int& outIdx, int& outFrom, int& outSteps) {
        std::lock_guard<std::mutex> lk(moveMx);
        if (!hasPending) return false;
        outIdx   = pendingMove.playerIdx;
        outFrom  = pendingMove.fromPos;
        outSteps = pendingMove.steps;
        hasPending = false;
        return true;
    }

private:
    struct PendingMove { int playerIdx, fromPos, steps; };

    GameManager& gm;
    mutable GuiStreamBuf  coutBuf;
    GuiInputBuf           cinBuf;
    std::streambuf*       oldCoutBuf = nullptr;
    std::streambuf*       oldCinBuf  = nullptr;

    mutable std::vector<TileInfo>    cachedTiles;
    mutable std::vector<PlayerInfo>  cachedPlayers;
    mutable std::vector<std::string> lineCache;

    std::atomic<bool>  diceRolling{false};
    std::atomic<float> diceTimer{0.f};

    mutable std::mutex moveMx;
    PendingMove pendingMove{};
    bool        hasPending = false;

    void buildTileCache() const {
        cachedTiles.clear();
        try {
            const Board& board = const_cast<GameManager&>(gm).getBoard();
            const auto& tiles  = board.getTiles();
            for (int i = 0; i < (int)tiles.size(); ++i) {
                Tile* t = tiles[i];
                if (!t) continue;
                TileInfo info{};
                info.pos  = i + 1;
                info.code = t->getCode();
                info.name = t->getName();
                fillTileTypeAndColor(info, t);
                info.ownerIdx = -1;
                cachedTiles.push_back(info);
            }
        } catch (...) {}  
    }

    void refreshTiles() const {
        if (cachedTiles.empty()) buildTileCache();
        if (cachedTiles.empty()) return;
        try {
            Board& board = const_cast<GameManager&>(gm).getBoard();
            for (auto& info : cachedTiles) {
                Tile* t = board.getTile(info.pos - 1);
                if (!t) continue;
                auto* pt = dynamic_cast<PropertyTile*>(t);
                if (!pt) continue;
                Property* prop = pt->getProperty();
                if (!prop) continue;
                info.ownerIdx      = -1;
                info.ownerName     = "";
                info.mortgaged     = prop->isMortgaged();
                info.festivalActive= (prop->getFestivalDuration() > 0);
                info.festivalMult  = (int)prop->getFestivalMultiplier();
                info.buildings     = 0;
                if (prop->getOwner()) {
                    const auto& all = const_cast<GameManager&>(gm).getPlayers();
                    for (int i = 0; i < (int)all.size(); ++i) {
                        if (&all[i] == prop->getOwner()) {
                            info.ownerIdx  = i;
                            info.ownerName = prop->getOwner()->getUsername();
                            break;
                        }
                    }
                }
                if (auto* sp = dynamic_cast<StreetProperty*>(prop)) {
                    info.buildings = sp->hasHotel() ? 5 : sp->getHouseCount();
                }
            }
        } catch (...) {}
    }

    void refreshPlayers() const {
        cachedPlayers.clear();
        try {
            int curIdx = currentPlayerIdx();
            const auto& all = const_cast<GameManager&>(gm).getPlayers();
            for (int i = 0; i < (int)all.size(); ++i) {
                const Player& p = all[i];
                PlayerInfo pi{};
                pi.idx      = i;
                pi.username = p.getUsername();
                pi.money    = p.getMoney();
                pi.isCurrent= (i == curIdx);
                switch (p.getState()) {
                    case PlayerState::ACTIVE:   pi.state = "ACTIVE";   break;
                    case PlayerState::JAILED:   pi.state = "JAILED";   break;
                    case PlayerState::BANKRUPT: pi.state = "BANKRUPT"; break;
                }
                Tile* cur = p.getPiece().getCurrentTile();
                if (cur) {
                    int tpos = const_cast<GameManager&>(gm).getBoard().getTilePosition(cur->getCode());
                    pi.pos = (tpos >= 0) ? tpos + 1 : 1;
                } else {
                    pi.pos = 1;
                }
                cachedPlayers.push_back(pi);
            }
        } catch (...) {}
    }

    static void fillTileTypeAndColor(TileInfo& info, Tile* t) {
        std::string code = t->getCode();
        if (code == "GO")  { info.tileType = "GO";           info.colorGroup = "DEFAULT"; return; }
        if (code == "PEN") { info.tileType = "JAIL";         info.colorGroup = "DEFAULT"; return; }
        if (code == "BBP") { info.tileType = "FREE_PARKING"; info.colorGroup = "DEFAULT"; return; }
        if (code == "PPJ") { info.tileType = "GO_TO_JAIL";   info.colorGroup = "DEFAULT"; return; }
        if (code == "PPH") { info.tileType = "INCOME_TAX";   info.colorGroup = "DEFAULT"; return; }
        if (code == "PBM") { info.tileType = "LUXURY_TAX";   info.colorGroup = "DEFAULT"; return; }
        if (code == "FES") { info.tileType = "FESTIVAL";     info.colorGroup = "DEFAULT"; return; }
        if (code == "DNU") { info.tileType = "COMMUNITY";    info.colorGroup = "DEFAULT"; return; }
        if (code == "KSP") { info.tileType = "CHANCE";       info.colorGroup = "DEFAULT"; return; }

        if (auto* pt = dynamic_cast<PropertyTile*>(t)) {
            Property* prop = pt->getProperty();
            if (!prop) { info.tileType = "STREET"; info.colorGroup = "DEFAULT"; return; }
            std::string type = prop->getPropertyType();
            if (type == "RAILROAD")      { info.tileType = "RAILROAD"; info.colorGroup = "DEFAULT"; }
            else if (type == "UTILITY")  { info.tileType = "UTILITY";  info.colorGroup = "ABU_ABU"; }
            else                         { info.tileType = "STREET";   info.colorGroup = prop->getColor(); }
            return;
        }
        info.tileType   = "DEFAULT";
        info.colorGroup = "DEFAULT";
    }
};