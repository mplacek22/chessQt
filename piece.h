#ifndef PIECE_H
#define PIECE_H

#include "Color.h"
#include "pieceType.h"
#include <memory>

class Piece
{
public:
    explicit Piece(Color color);

    static std::shared_ptr<Piece> create(Color color, PieceType pieceType);

    PieceType type() const { return type_; }
    Color color() const { return color_; }
    bool hasMoved() const { return hasMoved_; }
    char name() const { return name_; }

protected:
    PieceType type_;
    Color color_;
    bool hasMoved_ = false;
    char name_;
};

#endif // PIECE_H
