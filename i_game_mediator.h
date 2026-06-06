#pragma once

#include "DrawCause.h"
#include "Move.h"
#include "game_state.h"
#include <vector>


class IGameMediator
{
public:
    virtual ~IGameMediator() = default;

    virtual void onStartRequested() = 0;
    virtual void onMoveRequested(Move& move) = 0;
    virtual void onPromotionRequested(PieceType type) = 0;
    virtual void onRestartRequested() = 0;
    virtual void onPossibleMovesRequested(Coordinate coord) = 0;

    virtual void onGameStateChanged(const GameState& state) = 0;
    virtual void onGameWon(Color winner) = 0;
    virtual void onGameDrawn(DrawCause cause) = 0;
    virtual void onPromotionPending(bool isPromotionPending) = 0;
    virtual void onPossibleMovesCalculated(const std::vector<Move>& moves) = 0;
};

