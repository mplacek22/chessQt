#include "King.h"

std::span<const std::array<int, 2> > King::getMoveDirections() const {
    return MOVE_DIRECTIONS;
}

King::King(const Color color)
    : Piece(color, PieceType::KING) {}

std::unique_ptr<Piece> King::clone() const {
    return std::make_unique<King>(*this);
}

bool King::isSliding() const { return false; }
