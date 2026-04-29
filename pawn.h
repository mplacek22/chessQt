#pragma once

#include "Move.h"
#include "Piece.h"


class Pawn final : public Piece {
public:
    explicit Pawn(const Color color)
        : Piece(color) {
        type_ = PieceType::PAWN;
    }

    std::span<const std::array<int, 2>> getMoveDirections() const override {
        return {};
    }

    const std::array<int, 2> getNormalMoveDirections() const {
        return color_ == Color::WHITE ? DIRECTIONS_WHITE : DIRECTIONS_BLACK;
    }

    const std::span<const std::array<int, 2>> getCaptureMoveDirections() const override {
        return color_ == Color::WHITE ? CAPTURE_DIRECTIONS_WHITE : CAPTURE_DIRECTIONS_BLACK;
    }

    bool isSliding() const override { return false; }
private:
    static constexpr std::array<int, 2> DIRECTIONS_WHITE = {{ 1, 0}};
    static constexpr std::array<int, 2> DIRECTIONS_BLACK = {{-1, 0}};
    static constexpr std::array<std::array<int, 2>, 2> CAPTURE_DIRECTIONS_WHITE = {{{ -1, -1 }, { -1, 1 }}};
    static constexpr std::array<std::array<int, 2>, 2> CAPTURE_DIRECTIONS_BLACK = {{{  1, -1 }, {  1, 1 }}};


};
