#pragma once
#include "Piece.h"

/**
 * @class King
 * @brief Represents a bishop chess piece.
 *
 * A king can move 1 square in any direction.
 * It is not a sliding piece.
 */
class King final : public Piece {
public:
    explicit King(Color color);

    /**
     * @copydoc Piece::getMoveDirections()
     *
     * Bishop can move 1 square in any directions.
     */
    [[nodiscard]] std::span<const std::array<int, 2>> getMoveDirections() const override;

    /// @copydoc Piece::clone()
    [[nodiscard]] std::unique_ptr<Piece> clone() const override;;

    /**
    * @copydoc Piece::isSliding()
    *
    * King always returns false.
    */
    [[nodiscard]] bool isSliding() const override;

private:
    static constexpr std::array<std::array<int, 2>, 8> MOVE_DIRECTIONS = {{
        { 1,  0}, {-1,  0}, { 0,  1}, { 0, -1},
        { 1,  1}, { 1, -1}, {-1,  1}, {-1, -1}
    }};
};
