#ifndef BISHOP_H
#define BISHOP_H

#include <Piece.h>


class Bishop final : public Piece {
public:
    explicit Bishop(const Color color)
        : Piece(color) {
        type_ = PieceType::BISHOP;
        if (color == Color::WHITE)
            name_ = 'B';
        else
            name_ = 'b';
    }
};

#endif // BISHOP_H
