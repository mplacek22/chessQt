#ifndef ROOK_H
#define ROOK_H

#include <Piece.h>


class Rook final : public Piece {
public:
    explicit Rook(const Color color)
        : Piece(color, PieceType::ROOK) {}

    std::span<const std::array<int, 2>> getMoveDirections() const override {
        return MOVE_DIRECTIONS;
    }

    std::unique_ptr<Piece> clone() const override {
        return std::make_unique<Rook>(*this);
    };

    bool isSliding() const override { return true; }

private:
    static constexpr std::array<std::array<int, 2>, 4> MOVE_DIRECTIONS = {{
        { 1,  0}, {-1,  0}, { 0,  1}, { 0, -1}
    }};
};

#endif // ROOK_H
