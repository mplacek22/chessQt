#ifndef PAWN_H
#define PAWN_H

#include <Piece.h>
#include "Move.h"


class Pawn final : public Piece {
public:
    explicit Pawn(const Color color)
        : Piece(color) {
        type_ = PieceType::PAWN;
    }

    std::vector<std::shared_ptr<Move>> calculatePossibleMoves(
        const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board,
        const Coordinate& source,
        std::optional<Move> lastMove
    ) override;

    std::span<const std::array<int, 2>> getMoveDirections() const override {
        return {};
    }

    std::array<int, 2> getNormalMoveDirections() const {
        return color_ == Color::WHITE ? DIRECTIONS_WHITE : DIRECTIONS_BLACK;
    }

    const std::array<int, 2> getCaptureMoveDirections() const {
        return color_ == Color::WHITE ? DIRECTIONS_WHITE : DIRECTIONS_BLACK;
    }

    bool isSliding() const override { return false; }
private:
    static constexpr std::array<int, 2> DIRECTIONS_WHITE = {{ 1, 0}};
    static constexpr std::array<int, 2> DIRECTIONS_BLACK = {{-1, 0}};
    static constexpr std::array<int, 2> CAPTURE_DIRECTIONS_WHITE = {{1, 1}};
    static constexpr std::array<int, 2> CAPTURE_DIRECTIONS_BLACK = {{1, 1}};


};

#endif // PAWN_H
