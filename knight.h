#ifndef KNIGHT_H
#define KNIGHT_H

#include <Piece.h>


class Knight final : public Piece {
public:
    explicit Knight(Color color)
        : Piece(color) {
        type_ = PieceType::KNIGHT;
        name_ = 'N';
    }

    // std::vector<std::shared_ptr<Move> > calculatePossibleMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8> &board, const Coordinate &source) override;
    std::span<const std::array<int, 2> > getMoveDirections() const override {
        return DIRECTIONS;
    };
    bool isSliding() const override { return false; };

private:
    static constexpr std::array<std::array<int, 2>, 8> DIRECTIONS = {{
        { 2,  1}, { 2, -1}, {-2,  1}, {-2, -1},
        { 1,  2}, { 1, -2}, {-1,  2}, {-1, -2}
    }};
};

#endif // KNIGHT_H
