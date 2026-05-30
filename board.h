#pragma once

#include "Coordinate.h"
#include <memory>
#include "Grid.h"
#include "Piece.h"

/**
 * @brief class representing chess board.
 *
 */
class Board {
public:
    static constexpr int BOARD_SIZE = 8;

    Board();

    ~Board() = default;

    /**
     *
     * @param coordinate
     * @return
     */
    [[nodiscard]] std::shared_ptr<Piece> getPieceAt(const Coordinate &coordinate) const;

    void setPieceAt(const Coordinate &coordinate, std::shared_ptr<Piece> piece);

    void movePiece(const Coordinate &source, const Coordinate &destination);

    void restart();

    [[nodiscard]] const Grid<std::shared_ptr<Piece>>& board() const { return board_; }

    [[nodiscard]] Coordinate findKing(Color color) const;

    bool isFriendly(Coordinate& coord, Color color) const;

    bool isEnemy(Coordinate& coord, Color color) const;

    [[nodiscard]] bool inBounds(const Coordinate& coord) const;

    [[nodiscard]] bool isPathClear(const Coordinate& source, const Coordinate& destination) const;

    [[nodiscard]] bool isLightSquare(int rank, int file) const;

private:
    Grid<std::shared_ptr<Piece>> board_;

    void clear();

    void initialize();

};