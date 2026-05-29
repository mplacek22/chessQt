#pragma once

#include "Color.h"
#include "coordinate.h"
#include "MoveType.h"
#include "PieceType.h"
#include "gameStatus.h"
#include <optional>

struct Move {
    Coordinate source;
    Coordinate destination;
    MoveType moveType = MoveType::NORMAL;
    std::optional<PieceType> promotionPieceType;
    PieceType movingPieceType;
    Color player;
    bool isAmbiguous = false;
    bool isCapture = false;
    GameStatus gameStatusAfterMove;

    Move(const Coordinate source, const Coordinate destination, const Color currentPlayer): source(source),
        destination(destination), player(currentPlayer) {
    }
};