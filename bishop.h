#pragma once

#include "Piece.h"


class Bishop final : public Piece {
public:
    explicit Bishop(const Color color)
        : Piece(color) {
        type_ = PieceType::BISHOP;
        name_ = 'B';
    }

    std::span<const std::array<int, 2>> getMoveDirections() const override {
        return DIRECTIONS;
    }
    bool isSliding() const override { return true; }

private:
    static constexpr std::array<std::array<int, 2>, 4> DIRECTIONS = {{
        { 1,  1}, { 1, -1}, {-1,  1}, {-1, -1}
    }};
};