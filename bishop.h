#ifndef BISHOP_H
#define BISHOP_H

#include <Piece.h>


class Bishop final : public Piece {
public:
    explicit Bishop(const Color color)
        : Piece(color) {
        type_ = PieceType::BISHOP;
        name_ = 'B';
    }
};

#endif // BISHOP_H
