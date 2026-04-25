#pragma once

#include "Color.h"
#include "Move.h"
#include "PieceType.h"
#include "gameStatus.h"
#include <array>
#include <memory>
#include <optional>
#include <span>
#include <vector>

struct Move;
struct Coordinate;

class Piece
{
public:
    explicit Piece(Color color);

    PieceType type() const { return type_; }
    Color color() const { return color_; }
    bool hasMoved() const { return hasMoved_; }
    char name() const { return name_; }

    void setHasMoved(bool hasMoved) { hasMoved_ = hasMoved; }

    static std::shared_ptr<Piece> create(Color color, PieceType pieceType);

    virtual std::vector<std::shared_ptr<Move>> calculatePossibleMoves(
        const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board,
        const Coordinate& source,
        GameStatus gameStatus,
        std::optional<Move> lastMove = std::nullopt
    );

    std::vector<std::shared_ptr<Move>> calculatePseudoLegalMoves(
        const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board,
        const Coordinate& source
        );

    std::vector<Coordinate> computeCheckers(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board) const;

    // For non-sliding pieces (king, knight, pawn) these are single offsets.
    virtual std::span<const std::array<int, 2>> getMoveDirections() const = 0;

    // Sliding pieces follow a direction until blocked; non-sliding jump exactly once.
    virtual bool isSliding() const = 0;

protected:
    PieceType type_;
    Color color_;
    bool hasMoved_ = false;
    char name_;

    static const bool inBounds(int rank, int file) {
        return rank >= 0 && rank < 8 && file >= 0 && file < 8;
    }

    const bool isFriendly(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board, Coordinate& coord);
    const bool isEnemy(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board, Coordinate& coord);

private:
    Coordinate findKing(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board) const;
};
