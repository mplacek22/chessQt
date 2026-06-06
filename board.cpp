#include "board.h"
#include <Bishop.h>
#include <King.h>
#include <Knight.h>
#include <Queen.h>
#include <Rook.h>
#include <Pawn.h>

Board::Board() {
    initialize();
}

void Board::initialize() {
    Grid::set(0, 0, std::make_unique<Rook>(Color::WHITE));
    Grid::set(0, 1, std::make_unique<Knight>(Color::WHITE));
    Grid::set(0, 2, std::make_unique<Bishop>(Color::WHITE));
    Grid::set(0, 3, std::make_unique<Queen>(Color::WHITE));
    Grid::set(0, 4, std::make_unique<King>(Color::WHITE));
    Grid::set(0, 5, std::make_unique<Bishop>(Color::WHITE));
    Grid::set(0, 6, std::make_unique<Knight>(Color::WHITE));
    Grid::set(0, 7, std::make_unique<Rook>(Color::WHITE));

    for (int i = 0; i < 8; ++i) {
        Grid::set(6, i, std::make_unique<Pawn>(Color::WHITE));
        Grid::set(1, i, std::make_unique<Pawn>(Color::BLACK));
    }



    Grid::set(7, 0, std::make_unique<Rook>(Color::BLACK));
    Grid::set(7, 1, std::make_unique<Knight>(Color::BLACK));
    Grid::set(7, 2, std::make_unique<Bishop>(Color::BLACK));
    Grid::set(7, 3, std::make_unique<Queen>(Color::BLACK));
    Grid::set(7, 4, std::make_unique<King>(Color::BLACK));
    Grid::set(7, 5, std::make_unique<Bishop>(Color::BLACK));
    Grid::set(7, 6, std::make_unique<Knight>(Color::BLACK));
    Grid::set(7, 7, std::make_unique<Rook>(Color::BLACK));

    // // double check
    // // White pieces
    // Grid::set(0, 4, std::make_unique<King>(Color::WHITE));
    // Grid::set(4, 4, std::make_unique<Rook>(Color::WHITE));   // e5 – checks Black King on e8 along e-file
    // Grid::set(4, 1, std::make_unique<Bishop>(Color::WHITE)); // b5 – checks Black King on e8 via diagonal

    // // Black pieces
    // Grid::set(7, 4, std::make_unique<King>(Color::BLACK));   // e8 – in double check
    // Grid::set(7, 0, std::make_unique<Rook>(Color::BLACK));   // a8
    // Grid::set(7, 7, std::make_unique<Rook>(Color::BLACK));   // h8

    // almost check
    // White pieces
    // Grid::set(0, 4, std::make_unique<King>(Color::WHITE));
    // Grid::set(0, 0, std::make_unique<Rook>(Color::WHITE));    // Will give check on e-file
    // Grid::set(2, 6, std::make_unique<Bishop>(Color::WHITE));  // Will give check on diagonal

    // Black pieces
    // Grid::set(7, 4, std::make_unique<King>(Color::BLACK));    // Black King on e8
    // Grid::set(7, 0, std::make_unique<Rook>(Color::BLACK));    // Blocker removed to expose check
    // Grid::set(7, 7, std::make_unique<Rook>(Color::BLACK));

    // // pinned pieces
    // // White pieces
    // // Grid::set(0, 4, std::make_unique<King>(Color::WHITE));     // e1
    // // Grid::set(1, 4, std::make_unique<Knight>(Color::WHITE));   // e2 – pinned piece

    // // Black pieces
    // // Grid::set(7, 4, std::make_unique<Rook>(Color::BLACK));     // e8 – creates the pin
    // // Grid::set(6, 0, std::make_unique<King>(Color::BLACK));     // a7 – arbitrary safe placement

    // // pinned pieces 2
    // // White pieces
    // // Grid::set(0, 4, std::make_unique<King>(Color::WHITE));     // e1
    // // Grid::set(1, 4, std::make_unique<Bishop>(Color::WHITE));   // e2 – pinned piece

    // // Black pieces
    // // Grid::set(7, 4, std::make_unique<Rook>(Color::BLACK));     // e8 – creates the pin
    // // Grid::set(6, 0, std::make_unique<King>(Color::BLACK));     // a7 – arbitrary safe placement

    // almost checkmate
    // // White king
    // Grid::set(5, 5, std::make_unique<King>(Color::WHITE));   // f6

    // // White queen
    // Grid::set(5, 6, std::make_unique<Queen>(Color::WHITE));  // g6

    // // Black king
    // Grid::set(7, 7, std::make_unique<King>(Color::BLACK));   // h8

    // Grid::set(7, 1, std::make_unique<Knight>(Color::BLACK));


    // // castling
    // // --- WHITE (rank 0) ---
    // Grid::set(0, 0, std::make_unique<Rook>(Color::WHITE));     // a1 - unmoved, queen-side rook
    // Grid::set(0, 1, std::make_unique<Knight>(Color::WHITE));   // b1 - BLOCKS queen-side castle
    // // c1, d1 empty
    // Grid::set(0, 4, std::make_unique<King>(Color::WHITE));     // e1 - unmoved
    // // f1, g1 empty
    // Grid::set(0, 7, std::make_unique<Rook>(Color::WHITE));     // h1 - unmoved, king-side rook

    // // --- BLACK (rank 7) ---
    // Grid::set(7, 0, std::make_unique<Rook>(Color::BLACK));     // a8 - unmoved, queen-side rook
    // // b8, c8, d8 empty
    // Grid::set(7, 4, std::make_unique<King>(Color::BLACK));     // e8 - unmoved
    // Grid::set(7, 5, std::make_unique<Bishop>(Color::BLACK));   // f8 - BLOCKS king-side castle
    // Grid::set(7, 7, std::make_unique<Rook>(Color::BLACK));     // h8 - unmoved

    // // --- A white rook on e5 gives check to black king on e8, preventing king-side castle ---
    // Grid::set(4, 4, std::make_unique<Rook>(Color::WHITE));     // e5 - attacks e8, puts black in SINGLE_CHECK


    // insufficient material
    // Grid::set(0, 4, std::make_unique<King>(Color::WHITE));   // e1
    // Grid::set(1, 7, std::make_unique<Bishop>(Color::WHITE)); // h2 (light square)

    // Grid::set(7, 7, std::make_unique<King>(Color::BLACK));   // h8
    // Grid::set(4, 4, std::make_unique<Bishop>(Color::BLACK)); // e5 (light square) — capturable by Bd4

}

void Board::set(const Coordinate& coordinate, std::unique_ptr<Piece> piece) {
    Grid::set(coordinate.rank, coordinate.file,  std::move(piece));
}

void Board::move(const Coordinate& source, const Coordinate& destination) {
    Grid::move(source.rank, source.file, destination.rank, destination.file);
}

void Board::restart() {
    clear();
    initialize();
}

Coordinate Board::findKing(Color color) const
{
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int f = 0; f < BOARD_SIZE; f++) {
            const auto& piece = Grid::at(r, f);
            if (piece && piece->type() == PieceType::KING && piece->color() == color ) {
                return {r, f};
            }
        }
    }
    throw std::runtime_error("King not found - board state is invalid!");
}

bool Board::isFriendly(const Coordinate &coord, const Color color) const
{
    if (const auto target = at(coord)) {
        return target->color() == color;;
    }
    return false;
}

bool Board::isEnemy(const Coordinate &coord, const Color color) const
{
    if (const auto target = at(coord)) {
        return target->color() != color;;
    }
    return false;
}

bool Board::inBounds(const Coordinate &coord) {
    return Grid::inBounds(coord.rank, coord.file);
}

bool Board::isPathClear(const Coordinate& source, const Coordinate& destination) const
{
    const int dRank = destination.rank - source.rank;
    const int dFile = destination.file - source.file;

    const int stepRank = dRank == 0 ? 0 : dRank > 0 ? 1 : -1;
    const int stepFile = dFile == 0 ? 0 : dFile > 0 ? 1 : -1;

    int rank = source.rank + stepRank;
    int file = source.file + stepFile;

    while (rank != destination.rank || file != destination.file) {
        if (at({rank, file}) != nullptr) {
            return false;
        }
        rank += stepRank;
        file += stepFile;
    }
    return true;
}

bool Board::isLightSquare(const int rank, const int file) {
    if (!inBounds({rank, file}))
        throw std::runtime_error("Board::isLightSquare: invalid rank/file");
    return (rank + file) % 2 == 0;
}

Board Board::clone() const
{
    Board copy;
    for (std::size_t i = 0; i < SIZE; ++i) {
        copy.data[i] = data[i] ? data[i]->clone() : nullptr;
    }
    return copy;
}

Piece* Board::at(const Coordinate& coordinate) const {
    return Grid::at(coordinate.rank, coordinate.file).get();
}
