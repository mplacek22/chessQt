#ifndef KING_H
#define KING_H

#include <Piece.h>


class King final : public Piece {
public:
    explicit King(Color color)
        : Piece(color) {
        type_ = PieceType::KING;

        if (color == Color::WHITE)
            name_ = 'K';
        else
            name_ = 'k';
    }
};

#endif // KING_H
