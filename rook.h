#ifndef ROOK_H
#define ROOK_H

#include <Piece.h>


class Rook final : public Piece {
public:
    explicit Rook(const Color color)
        : Piece(color) {
        type_ = PieceType::ROOK;
        // isPathClearRequired_ = true;
        if (color == Color::WHITE)
            name_ = 'R';
        else
            name_ = 'r';
    }
};

#endif // ROOK_H
