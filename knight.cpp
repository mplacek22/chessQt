#include "knight.h"


std::span<const std::array<int, 2> > Knight::getMoveDirections() const {
    return DIRECTIONS;
}

Knight::Knight(Color color)
    : Piece(color, PieceType::KNIGHT) {}

std::unique_ptr<Piece> Knight::clone() const {
    return std::make_unique<Knight>(*this);
}

bool Knight::isSliding() const { return false; }
