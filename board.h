//
// Created by User on 04.03.2025.
//

#ifndef BOARD_H
#define BOARD_H
#include "Coordinate.h"
#include <memory>
#include <array>
#include <vector>
#include "Grid.h"
#include "Piece.h"


class Board {
public:
    static constexpr int BOARD_SIZE = 8;

    Board();

    ~Board() = default;

    void initialize();

    void display() const;

    [[nodiscard]] std::shared_ptr<Piece> getPieceAt(const Coordinate &coordinate) const;

    void setPieceAt(const Coordinate &coordinate, std::shared_ptr<Piece> piece);

    void movePiece(const Coordinate &source, const Coordinate &destination);

    void restart();

    Grid<std::shared_ptr<Piece>, BOARD_SIZE, BOARD_SIZE> board() const { return board_; };

    Coordinate findKing(Color color) const;

    bool isFriendly(Coordinate& coord, Color color) const;

    bool isEnemy(Coordinate& coord, Color color) const;

    bool inBounds(const Coordinate& coord) const;

    bool isPathClear(const Coordinate& source, const Coordinate& destination) const;

    bool isLightSquare(int rank, int file) const;

private:
    Grid<std::shared_ptr<Piece>, BOARD_SIZE, BOARD_SIZE> board_;
    void clear();
};


#endif //BOARD_H
