#pragma once
#include <string>
#include <vector>

struct TileInfo {
    int         pos;
    std::string code;
    std::string name; 
    std::string colorGroup;
    std::string tileType;
    std::string ownerName;    // "" = bank/none
    int ownerIdx;     // -1 = none
    int buildings;    // 0-4 houses, 5 = hotel
    bool mortgaged;
    bool festivalActive;
    int festivalMult;
};

struct PlayerInfo {
    int idx;                   // 0-based
    std::string username;
    int pos;                   // 1-based board position
    long long money;
    std::string state;        // "ACTIVE"|"JAILED"|"BANKRUPT"
    bool isCurrent;
};

struct DiceInfo {
    int d1 = 1, d2 = 1;
    bool rolling = false;
    float rollTimer = 0.f;
};

class IGuiState {
public:
    virtual ~IGuiState() = default;

    virtual const std::vector<TileInfo>&   tiles() const = 0;
    virtual const std::vector<PlayerInfo>& players() const = 0;
    virtual int currentPlayerIdx()const = 0;
    virtual int currentTurn() const = 0;
    virtual int maxTurn() const = 0;
    virtual DiceInfo dice() const = 0;
    virtual const std::vector<std::string>& consoleLines() const = 0;

    virtual void submitCommand(const std::string& cmd) = 0;
};
