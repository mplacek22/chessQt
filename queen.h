#ifndef QUEEN_H
#define QUEEN_H

#include <Piece.h>


class Queen final : public Piece {
public:
    explicit Queen(Color color)
        : Piece(color) {
        type_ = PieceType::QUEEN;
        name_ = 'Q';
    }
};

#endif // QUEEN_H
