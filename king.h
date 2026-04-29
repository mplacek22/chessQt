#pragma once
#include "Piece.h"


class King final : public Piece {
public:
    explicit King(Color color)
        : Piece(color) {
        type_ = PieceType::KING;
        name_ = 'K';
    }

    std::span<const std::array<int, 2>> getMoveDirections() const override {
        return MOVE_DIRECTIONS;
    }

    bool isSliding() const override { return false; }

private:
    static constexpr std::array<std::array<int, 2>, 8> MOVE_DIRECTIONS = {{
        { 1,  0}, {-1,  0}, { 0,  1}, { 0, -1},
        { 1,  1}, { 1, -1}, {-1,  1}, {-1, -1}
    }};
};
