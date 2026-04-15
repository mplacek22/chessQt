#ifndef KING_H
#define KING_H

#include <Piece.h>


class King final : public Piece {
public:
    explicit King(Color color)
        : Piece(color) {
        type_ = PieceType::KING;
        name_ = 'K';
    }

    std::vector<std::shared_ptr<Move>> calculatePossibleMoves(
        const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board,
        const Coordinate& source,
        std::optional<Move> lastMove
    ) override;

    std::span<const std::array<int, 2>> getMoveDirections() const override {
        return MOVE_DIRECTIONS;
    }

    bool isSliding() const override { return false; }

private:
    static constexpr std::array<std::array<int, 2>, 8> MOVE_DIRECTIONS = {{
        { 1,  0}, {-1,  0}, { 0,  1}, { 0, -1},
        { 1,  1}, { 1, -1}, {-1,  1}, {-1, -1}
    }};

    std::vector<Coordinate> computeKingDangerSquares(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board,
                                      const Coordinate& kingPosition) const;


};

#endif // KING_H
