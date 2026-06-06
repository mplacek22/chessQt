#include "Bishop.h"

std::span<const std::array<int, 2> > Bishop::getMoveDirections() const {
    return DIRECTIONS;
}

Bishop::Bishop(const Color color)
    : Piece(color, PieceType::BISHOP) {}

std::unique_ptr<Piece> Bishop::clone() const {
    return std::make_unique<Bishop>(*this);
}

bool Bishop::isSliding() const { return true; }