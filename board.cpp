#include "board.h"

Board::Board() {
    initialize();
}

void Board::initialize() {
    set({0, 0}, {PieceType::ROOK,   Color::WHITE});
    set({0, 1}, {PieceType::KNIGHT, Color::WHITE});
    set({0, 2}, {PieceType::BISHOP, Color::WHITE});
    set({0, 3}, {PieceType::QUEEN,  Color::WHITE});
    set({0, 4}, {PieceType::KING,   Color::WHITE});
    set({0, 5}, {PieceType::BISHOP, Color::WHITE});
    set({0, 6}, {PieceType::KNIGHT, Color::WHITE});
    set({0, 7}, {PieceType::ROOK,   Color::WHITE});

    for (int i = 0; i < 8; ++i) {
        set({1, i}, {PieceType::PAWN, Color::WHITE});
        set({6, i}, {PieceType::PAWN, Color::BLACK});
    }

    set({7, 0}, {PieceType::ROOK,   Color::BLACK});
    set({7, 1}, {PieceType::KNIGHT, Color::BLACK});
    set({7, 2}, {PieceType::BISHOP, Color::BLACK});
    set({7, 3}, {PieceType::QUEEN,  Color::BLACK});
    set({7, 4}, {PieceType::KING,   Color::BLACK});
    set({7, 5}, {PieceType::BISHOP, Color::BLACK});
    set({7, 6}, {PieceType::KNIGHT, Color::BLACK});
    set({7, 7}, {PieceType::ROOK,   Color::BLACK});
}

void Board::set(const Coordinate& coordinate, std::optional<Piece> piece) {
    Grid::set(coordinate.rank, coordinate.file, piece);
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
            const auto piece = at({r, f});
            if (piece.has_value() && piece->type == PieceType::KING && piece->color == color ) {
                return {r, f};
            }
        }
    }
    throw std::runtime_error("King not found - board state is invalid!");
}

bool Board::isFriendly(const Coordinate &coord, const Color color) const
{
    if (const auto target = at(coord)) {
        return target->color == color;;
    }
    return false;
}

bool Board::isEnemy(const Coordinate &coord, const Color color) const
{
    if (const auto target = at(coord)) {
        return target->color != color;;
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
        if (at({rank, file}).has_value()) {
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

std::optional<Piece>& Board::at(const Coordinate& coordinate) {
    return Grid::at(coordinate.rank, coordinate.file);
}

const std::optional<Piece> &Board::at(const Coordinate &coordinate) const
{
    return Grid::at(coordinate.rank, coordinate.file);
}

void Board::set(const Coordinate &coordinate, const Piece &piece)
{
    Grid::set(coordinate.rank, coordinate.file, std::optional<Piece>{piece});
}