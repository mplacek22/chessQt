//
// Created by User on 04.03.2025.
//

#ifndef BOARD_H
#define BOARD_H
#include "Coordinate.h"
#include <memory>
#include <array>
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


    [[nodiscard]] bool isPathClear(Coordinate source, Coordinate destination) const;

private:
    std::array<std::array<std::shared_ptr<Piece>, BOARD_SIZE>, BOARD_SIZE> board_;
    void clear();
};


#endif //BOARD_H
