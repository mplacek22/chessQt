#ifndef ROOK_H
#define ROOK_H

#include <Piece.h>


class Rook final : public Piece {
public:
    explicit Rook(const Color color)
        : Piece(color) {
        type_ = PieceType::ROOK;
        name_ = 'R';
    }

    std::span<const std::array<int, 2>> getMoveDirections() const override {
        return MOVE_DIRECTIONS;
    }

    bool isSliding() const override { return true; }

private:
    static constexpr std::array<std::array<int, 2>, 4> MOVE_DIRECTIONS = {{
        { 1,  0}, {-1,  0}, { 0,  1}, { 0, -1}
    }};
};

#endif // ROOK_H
