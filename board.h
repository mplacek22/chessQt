#pragma once

#include "Coordinate.h"
#include <memory>
#include "Grid.h"
#include "Piece.h"

/**
 * @brief class representing chess board.
 *
 */
class Board : public Grid<std::unique_ptr<Piece>, 8, 8> {
public:
    static constexpr int BOARD_SIZE = 8;

    Board();

    ~Board() = default;

    Board(Board&&) = default;
    Board& operator=(Board&&) = default;
    /**
     *
     * @param coordinate
     * @return
     */
    [[nodiscard]] Piece* at(const Coordinate &coordinate) const;

    void set(const Coordinate &coordinate, std::unique_ptr<Piece> piece);

    void move(const Coordinate &source, const Coordinate &destination);

    void restart();

    [[nodiscard]] Coordinate findKing(Color color) const;

    bool isFriendly(Coordinate& coord, Color color) const;

    bool isEnemy(Coordinate& coord, Color color) const;

    [[nodiscard]] bool inBounds(const Coordinate& coord) const;

    [[nodiscard]] bool isPathClear(const Coordinate& source, const Coordinate& destination) const;

    [[nodiscard]] bool isLightSquare(int rank, int file) const;

    [[nodiscard]] Board clone() const;

private:
    void initialize();

};