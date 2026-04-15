#pragma once

#include "coordinate.h"
#include "Color.h"
#include "MoveType.h"
#include "gameStatus.h"
#include "PieceType.h"
#include <memory>
#include <optional>

class Piece;

struct Move {
    Coordinate source;
    Coordinate destination;
    MoveType moveType = MoveType::NORMAL;
    std::optional<PieceType> promotionPieceType;
    std::shared_ptr<Piece> movingPiece = nullptr;
    std::shared_ptr<Piece> capturedPiece = nullptr;
    Color player;
    GameStatus gameStatus;
    bool isAmbiguous = false;

    Move(const Coordinate source, const Coordinate destination, const Color currentPlayer): source(source),
        destination(destination), player(currentPlayer) {
    }

    static const std::string toChessNotation(const Move& move);
};