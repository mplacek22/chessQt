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
    Piece(const Color color, const PieceType type)
        : color_(color), type_(type) {}

    virtual ~Piece() = default;

    virtual std::unique_ptr<Piece> clone() const = 0;

    [[nodiscard]] PieceType type() const;

    [[nodiscard]] Color color() const;

    [[nodiscard]] bool hasMoved() const;

    void setHasMoved(bool hasMoved);

    static std::unique_ptr<Piece> create(Color color, PieceType pieceType);

    // For non-sliding pieces (king, knight, pawn) these are single offsets.
    [[nodiscard]] virtual std::span<const std::array<int, 2>> getMoveDirections() const = 0;

    // Sliding pieces follow a direction until blocked; non-sliding jump exactly once.
    [[nodiscard]] virtual bool isSliding() const = 0;

protected:
    const PieceType type_;
    const Color color_;
    bool hasMoved_ = false;
};
