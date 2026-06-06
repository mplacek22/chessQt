#pragma once

#include "Piece.h"

/**
 * @class Bishop
 * @brief Represents a bishop chess piece.
 *
 * A bishop moves any number of squares diagonally (in 4 directions).
 * It is a sliding piece.
 */
class Bishop final : public Piece {
public:
    explicit Bishop(Color color);

    /**
     * @copydoc Piece::getMoveDirections()
     *
     * Bishop can move in all 4 diagonal directions.
     */
    [[nodiscard]] std::span<const std::array<int, 2>> getMoveDirections() const override;


    /// @copydoc Piece::clone()
    [[nodiscard]] std::unique_ptr<Piece> clone() const override;;

    /**
    * @copydoc Piece::isSliding()
    *
    * Bishop always returns true.
    */
    [[nodiscard]] bool isSliding() const override;

private:
    static constexpr std::array<std::array<int, 2>, 4> DIRECTIONS = {{
        { 1,  1}, { 1, -1}, {-1,  1}, {-1, -1}
    }};
};