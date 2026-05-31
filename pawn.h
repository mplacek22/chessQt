#pragma once

#include "Piece.h"


class Pawn final : public Piece {
public:
    explicit Pawn(const Color color)
        : Piece(color, PieceType::PAWN) {}

    // Piece interface
    std::span<const std::array<int, 2>> getMoveDirections() const override {
        return {};
    }

    std::unique_ptr<Piece> clone() const override {
        return std::make_unique<Pawn>(*this);
    };

    const std::array<int, 2> getNormalMoveDirections() const {
        return color_ == Color::WHITE ? DIRECTIONS_WHITE : DIRECTIONS_BLACK;
    }

    bool isSliding() const override { return false; }
private:
    static constexpr std::array<int, 2> DIRECTIONS_WHITE = {{ 1, 0}};
    static constexpr std::array<int, 2> DIRECTIONS_BLACK = {{-1, 0}};
    static constexpr std::array<std::array<int, 2>, 2> CAPTURE_DIRECTIONS_WHITE = {{{ -1, -1 }, { -1, 1 }}};
    static constexpr std::array<std::array<int, 2>, 2> CAPTURE_DIRECTIONS_BLACK = {{{  1, -1 }, {  1, 1 }}};
};
