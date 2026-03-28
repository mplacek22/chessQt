#ifndef ROOK_H
#define ROOK_H

#include <Piece.h>


class Rook final : public Piece {
public:
    explicit Rook(const Color color)
        : Piece(color) {
        type_ = PieceType::ROOK;
        name_ = 'R';
        // isPathClearRequired_ = true;
    }
};

#endif // ROOK_H
