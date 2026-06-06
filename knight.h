#pragma once

#include "Piece.h"

/**
 * @class Knight
 * @brief Represents a bishop chess piece.
 *
 * A bishop moves any number of squares diagonally (in 4 directions).
 * It is a sliding piece.
 */
class Knight final : public Piece {
public:
    explicit Knight(Color color);

    /**
     * @copydoc Piece::getMoveDirections()
     *
     * Knight either moves up or down one square vertically and over two squares horizontally
     * OR up or down two squares vertically and over one square horizontally.
     * This movement can be remembered as an "L-shape" because it looks like a capital "L".
     */
    [[nodiscard]] std::span<const std::array<int, 2> > getMoveDirections() const override;;

    /// @copydoc Piece::clone()
    [[nodiscard]] std::unique_ptr<Piece> clone() const override;;

    /**
    * @copydoc Piece::isSliding()
    *
    * Knight always returns false.
    */
    [[nodiscard]] bool isSliding() const override;;

private:
    static constexpr std::array<std::array<int, 2>, 8> DIRECTIONS = {{
        { 2,  1}, { 2, -1}, {-2,  1}, {-2, -1},
        { 1,  2}, { 1, -2}, {-1,  2}, {-1, -2}
    }};
};