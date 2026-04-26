#pragma once

#include <vector>
#include <queue>
#include <functional>
#include <utility>

class PieceAnimator {
public:
    explicit PieceAnimator(int maxPlayers = 4, int boardSize = 40);
    void movePiece(int playerIdx, int fromPos, int steps, std::function<void()> onComplete = nullptr);
    void setPos(int playerIdx, int pos);
    void update(float dt);
    void gridPos(int playerIdx, float& col, float& row) const;
    float jumpOffset(int playerIdx) const;
    bool anyAnimating() const;
    bool isAnimating(int playerIdx) const;

    static std::pair<int,int> posToGrid(int pos);

private:
    struct Hop {
        int   from = 1, to = 1;
        float t= 1.f;
    };
    struct Anim {
        int  curPos = 1;
        bool active = false;
        Hop  hop;
        std::queue<int> pending;
        std::function<void()> onDone;
    };
    void kickNextHop(int idx);

    int  boardSize;
    std::vector<Anim> acceleration;

    static constexpr float SPEED = 8.f;   // hops / sec
    static constexpr float JUMP  = 24.f;  // in pixel 
};
