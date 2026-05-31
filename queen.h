#pragma once

#include <Piece.h>


class Queen final : public Piece {
public:
    explicit Queen(Color color)
        : Piece(color, PieceType::QUEEN) {}

    std::span<const std::array<int, 2>> getMoveDirections() const override {
        return MOVE_DIRECTIONS;
    }

    std::unique_ptr<Piece> clone() const override {
        return std::make_unique<Queen>(*this);
    };

    bool isSliding() const override { return true; }

private:
    static constexpr std::array<std::array<int, 2>, 8> MOVE_DIRECTIONS = {{
        { 1,  0}, {-1,  0}, { 0,  1}, { 0, -1},
        { 1,  1}, { 1, -1}, {-1,  1}, {-1, -1}
    }};
};
