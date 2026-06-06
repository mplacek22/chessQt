#pragma once

#include "Coordinate.h"
#include <memory>
#include "Grid.h"
#include "Piece.h"

/**
 * @class Board
 *
 * @brief Represents the chess board.
 */
class Board : public Grid<std::unique_ptr<Piece>> {
public:
    static constexpr int BOARD_SIZE = 8;

    Board();

    ~Board() = default;

    Board(Board&&) = default;

    Board& operator=(Board&&) = default;

    /**
     * @brief Returns a piece placed at the specified coordinate.
     * @param coordinate Board coordinate to be inspected.
     * @return Pointer to the piece at the coordinate or nullptr if the square is empty
     */
    [[nodiscard]] Piece* at(const Coordinate &coordinate) const;

    /**
     * @brief Sets a piece at the specified coordinate.
     *
     * Any existing piece on the square is replaced.
     *
     * @param coordinate Destination square.
     * @param piece Piece to be placed.
     */
    void set(const Coordinate &coordinate, std::unique_ptr<Piece> piece);

    /**
     * @brief Moves a piece from source square to destination square
     *
     * Any existing piece on the destination square is replaced (captured).
     * @param source Source square
     * @param destination Destination square
     */
    void move(const Coordinate &source, const Coordinate &destination);

    /**
     * @brief Resets the board to initial state
     */
    void restart();

    /**
     * @brief Finds the king of the specified color.
     *
     * The king must be always present on the board.
     *
     * @param color Color of the king to be found.
     * @throws std::runtime_error if king is not found.
     * @return Coordinate of the king.
     */
    [[nodiscard]] Coordinate findKing(Color color) const;

    /**
    * @brief Checks whether a square contains a friendly piece.
    *
    * @param coord Coordinate to be inspected.
    * @param color Color to compare against.
    * @return @a true if the square contains a piece of the given color, @a false otherwise.
    */
    [[nodiscard]] bool isFriendly(const Coordinate& coord, Color color) const;

     /**
     * @brief Checks whether a square contains an enemy piece.
     *
     * @param coord Coordinate to be inspected.
     * @param color Color to compare against.
     * @return @a true if the square contains a piece of the given color, @a false otherwise.
     */
    [[nodiscard]] bool isEnemy(const Coordinate& coord, Color color) const;

    /**
     * @brief Checks whether a square is in bounds of the board.
     *
     * Rank and file must be between 0 and 8.
     * @param coord Coordinate to be inspected.
     * @return @a true if the coordinate is in bounds, @a false otherwise.
     */
    [[nodiscard]] static bool inBounds(const Coordinate& coord);

    /**
     * @brief Checks whether a path is clear between two coordinates.
     *
     * A path is clear when no piece is present between source and destination coordinate.
     * @param source Coordinate of the start of the path to be checked.
     * @param destination Coordinate of the end of the path to be checked.
     * @return @a true if the path is clear, @a false otherwise
     */
    [[nodiscard]] bool isPathClear(const Coordinate& source, const Coordinate& destination) const;

    /**
     * @brief checks whether a square is light.
     *
     * In chess colors of the squares alternates between light and dark.
     * a1 is always light.
     *
     * @throws std::runtime_error If either @p rank or @p file is outside the valid range.
     * @param rank Rank of the square to be inspected.
     * @param file File of the square to be inspected.
     * @return @a true is square is light, @a false if dark.
     */
    [[nodiscard]] static bool isLightSquare(int rank, int file);

    /**
     * @brief Creates a deep copy of the board.
     *
     * Constructs a new Board instance containing copies of all pieces on this board
     * in their current positions.
     *
     * @return A deep copy of this board.
     */
    [[nodiscard]] Board clone() const;

private:
    void initialize();

};