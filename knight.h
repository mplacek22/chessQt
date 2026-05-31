#ifndef KNIGHT_H
#define KNIGHT_H

#include <Piece.h>


class Knight final : public Piece {
public:
    explicit Knight(Color color)
        : Piece(color, PieceType::KNIGHT) {}

    std::span<const std::array<int, 2> > getMoveDirections() const override {
        return DIRECTIONS;
    };

    std::unique_ptr<Piece> clone() const override {
        return std::make_unique<Knight>(*this);
    };

    bool isSliding() const override { return false; };

private:
    static constexpr std::array<std::array<int, 2>, 8> DIRECTIONS = {{
        { 2,  1}, { 2, -1}, {-2,  1}, {-2, -1},
        { 1,  2}, { 1, -2}, {-1,  2}, {-1, -2}
    }};
};

#endif // KNIGHT_H
