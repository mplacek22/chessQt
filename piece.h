#pragma once

#include "Color.h"
#include "PieceType.h"
#include <array>
#include <memory>
#include <span>

/**
 * @brief Abstract base class representing chess piece
 */
class Piece
{
public:
    /**
     * @brief Constructs a Piece with the given color
     * @param color The color of the piece
     */
    explicit Piece(Color color);

    [[nodiscard]] PieceType type() const { return type_; }
    [[nodiscard]] Color color() const { return color_; }
    [[nodiscard]] bool hasMoved() const { return hasMoved_; }

    void setHasMoved(const bool hasMoved) { hasMoved_ = hasMoved; }

    static std::shared_ptr<Piece> create(Color color, PieceType pieceType);

    // For non-sliding pieces (king, knight, pawn) these are single offsets.
    [[nodiscard]] virtual std::span<const std::array<int, 2>> getMoveDirections() const = 0;

    // Sliding pieces follow a direction until blocked; non-sliding jump exactly once.
    [[nodiscard]] virtual bool isSliding() const = 0;

    [[nodiscard]] virtual std::span<const std::array<int, 2>> getCaptureMoveDirections() const {
        return getMoveDirections();
    }

protected:
    ~Piece() = default;

    PieceType type_;
    Color color_;
    bool hasMoved_ = false;
};
