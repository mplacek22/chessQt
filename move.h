#ifndef MOVE_H
#define MOVE_H
#include <coordinate.h>

#include <memory>
#include <optional>
#include <Piece.h>
#include "Color.h"
#include "MoveType.h"

struct Move {
    Coordinate source;
    Coordinate destination;
    MoveType moveType = MoveType::NORMAL;
    std::optional<PieceType> promotionPieceType;
    std::shared_ptr<Piece> movingPiece = nullptr;
    std::shared_ptr<Piece> capturedPiece = nullptr;
    Color player;

    Move(const Coordinate source, const Coordinate destination, const Color currentPlayer): source(source),
        destination(destination), player(currentPlayer) {
    }
};


#endif //MOVE_H