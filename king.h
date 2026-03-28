#ifndef KING_H
#define KING_H

#include <Piece.h>


class King final : public Piece {
public:
    explicit King(Color color)
        : Piece(color) {
        type_ = PieceType::KING;
        name_ = 'K';
    }
};

#endif // KING_H
