#pragma once

#include "Color.h"
#include "PieceType.h"

/**
 * @struct Piece
 * @brief Abstract base class representing chess piece
 */
struct Piece
{
    Piece(const PieceType pieceType, const Color color) : type(pieceType), color(color) {};

    /// Type of the chess piece.
    PieceType type;

    /// Color of the chess piece.
    Color color;

    /// Indicates whether the piece has moved at least once.
    bool hasMoved = false;
};
