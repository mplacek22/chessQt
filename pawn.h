#ifndef PAWN_H
#define PAWN_H

#include <Piece.h>


class Pawn final : public Piece {
public:
    explicit Pawn(const Color color)
        : Piece(color) {
        type_ = PieceType::PAWN;
        if (color == Color::WHITE)
            name_ = 'P';
        else
            name_ = 'p';
    }
};

#endif // PAWN_H
