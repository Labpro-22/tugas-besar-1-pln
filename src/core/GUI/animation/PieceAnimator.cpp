#include "animation/PieceAnimator.hpp"
#include <cmath>

std::pair<int,int> PieceAnimator::posToGrid(int pos) {
    if (pos >= 1  && pos <= 11) return {11 - pos,  10};
    if (pos >= 12 && pos <= 20) return {0,          21 - pos};
    if (pos >= 21 && pos <= 31) return {pos - 21,   0};
    if (pos >= 32 && pos <= 40) return {10,         pos - 31};
    return {0, 0};
}

PieceAnimator::PieceAnimator(int maxPlayers, int boardSize): boardSize(boardSize), acceleration(maxPlayers){}

void PieceAnimator::setPos(int idx, int pos) {
    if (idx < 0 || idx >= (int) acceleration.size()) return;
    auto& acc = acceleration[idx];
    acc.active = false;
    acc.curPos = pos;
    acc.hop.from= pos;
    acc.hop.to = pos;
    acc.hop.t = 1.f;
    while (!acc.pending.empty()) acc.pending.pop();
}

void PieceAnimator::movePiece(int idx, int fromPos, int steps, std::function<void()> onComplete) {
    if (idx < 0 || idx >= (int)acceleration.size()) return;
    auto& acc = acceleration[idx];
    while (!acc.pending.empty()) acc.pending.pop();
    for (int s = 1; s <= steps; ++s)
        acc.pending.push(((fromPos - 1 + s) % boardSize) + 1);
    acc.curPos  = fromPos;
    acc.active  = true;
    acc.onDone  = onComplete;
    kickNextHop(idx);
}

void PieceAnimator::kickNextHop(int idx) {
    auto& acc = acceleration[idx];
    if (acc.pending.empty()) {
        acc.active = false;
        if (acc.onDone) { acc.onDone(); acc.onDone = nullptr; }
        return;
    }
    acc.hop.from = acc.curPos;
    acc.hop.to = acc.pending.front();
    acc.pending.pop();
    acc.hop.t = 0.f;
}

void PieceAnimator::update(float dt) {
    for (int i = 0; i < (int)acceleration.size(); ++i) {
        auto& acc = acceleration[i];
        if (!acc.active) continue;
        acc.hop.t += dt * SPEED;
        if (acc.hop.t >= 1.f) {
            acc.hop.t  = 1.f;
            acc.curPos = acc.hop.to;
            kickNextHop(i);
        }
    }
}

void PieceAnimator::gridPos(int idx, float& col, float& row) const {
    if (idx < 0 || idx >= (int)acceleration.size()) { col = 0; row = 0; return; }
    const auto& acc = acceleration[idx];
    auto [fc, fr] = posToGrid(acc.hop.from);
    auto [tc, tr] = posToGrid(acc.hop.to);
    float t = acc.hop.t;
    col = (float)fc + ((float)tc - (float)fc) * t;
    row = (float)fr + ((float)tr - (float)fr) * t;
}

float PieceAnimator::jumpOffset(int idx) const {
    if (idx < 0 || idx >= (int)acceleration.size()) return 0.f;
    const auto& acc = acceleration[idx];
    if (!acc.active && acc.hop.from == acc.hop.to) return 0.f;
    return JUMP * std::sin(acc.hop.t * 3.14159f);
}

bool PieceAnimator::anyAnimating() const {
    for (const auto& a : acceleration) if (a.active) return true;
    return false;
}
bool PieceAnimator::isAnimating(int idx) const {
    if (idx < 0 || idx >= (int)acceleration.size()) return false;
    return acceleration[idx].active;
}
