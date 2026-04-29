#pragma once

#include "Color.h"
#include "PieceType.h"
#include <array>
#include <memory>
#include <span>

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

    // For non-sliding pieces (king, knight, pawn) these are single offsets.
    virtual std::span<const std::array<int, 2>> getMoveDirections() const = 0;

    // Sliding pieces follow a direction until blocked; non-sliding jump exactly once.
    virtual bool isSliding() const = 0;

    // When a piece is pinned, it can only move towards or away from the pinner, it can’t leave the line between the attacking piece and the king.
    // virtual bool isPinned(std::array<std::array<std::shared_ptr<Piece>, 8>, 8> board) const = 0;

    virtual const std::span<const std::array<int, 2>> getCaptureMoveDirections() const {
        return getMoveDirections();
    }

protected:
    PieceType type_;
    Color color_;
    bool hasMoved_ = false;
    char name_;
};
