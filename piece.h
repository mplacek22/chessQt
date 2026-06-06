#pragma once

#include "Color.h"
#include "PieceType.h"
#include <array>
#include <memory>
#include <span>

/**
 * @class Piece
 * @brief Abstract base class representing chess piece
 */
class Piece
{
public:
    /**
     * @brief Constructs a Piece with the given color and type
     * @param color The color of the piece
     * @param type The type of the piece
     */
    Piece(const Color color, const PieceType type)
        : type_(type), color_(color) {}

    virtual ~Piece() = default;

    /**
     * @brief Creates a copy of this piece.
     * @return std::unique_ptr to the copy of this piece.
     */
    [[nodiscard]] virtual std::unique_ptr<Piece> clone() const = 0;

    /**
     * @brief Returns piece type
     * @return piece type
     */
    [[nodiscard]] PieceType type() const;

    /**
     * @brief Returns piece color (player)
     * @return piece color
     */
    [[nodiscard]] Color color() const;

    /**
     * @brief Returns whether the piece has moved.
     * @return @c true if the piece moved
     */
    [[nodiscard]] bool hasMoved() const;

    /**
     * @brief Sets whether the piece has moved
     * @param hasMoved has the piece moved
     */
    void setHasMoved(bool hasMoved);

    /**
     * @brief Creates a unique pointer owning a new piece of the specified color and type.
     *
     * Factory method that constructs and returns an instance of the requested piece type.
     *
     * @param color Piece color.
     * @param pieceType Piece type.
     * @return Unique pointer owning the newly created piece, or nullptr if the type is invalid.
     */
    static std::unique_ptr<Piece> create(Color color, PieceType pieceType);

    /**
     * @brief Returns the move directions supported by this pieces type.
     *
     * Move direction is represented as {rankDirection, fileDirection}
     * describing a movement vector.
     * Direction can be:
     * - +int forward
     * - -int backward
     * - 0 no movement
     *
     * Example:
     * {1, 0} - one a step forward on rank axis, no movement on rank axis.
     * {-1, 2} - one step backwards on rank axis, 2 steps forward on file axis.
     *
     * @return A span containing the movement direction vectors
     */
    [[nodiscard]] virtual std::span<const std::array<int, 2>> getMoveDirections() const = 0;

     /**
     * @brief Returns whether a piece is a sliding piece
     *
     * Sliding pieces (rook, bishop, queen) follow a direction until blocked.
     * Non-sliding (pawn, king, knight) pieces jump exactly once.
     *
     * @return @a true if the piece is a sliding piece.
     */
    [[nodiscard]] virtual bool isSliding() const = 0;

protected:
    /// Type of the chess piece.
    const PieceType type_;

    /// Color of the chess piece.
    const Color color_;

    /// Indicates whether the piece has moved at least once.
    bool hasMoved_ = false;
};
