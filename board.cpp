#include "board.h"

#include <iostream>

#include <Bishop.h>
#include <King.h>
#include <Knight.h>
#include <Queen.h>
#include <Rook.h>
// #include <Piece.h>
#include <Pawn.h>

Board::Board() {
    initialize();
}

void Board::initialize() {
    board_[0][0] = std::make_shared<Rook>(Color::WHITE);
    board_[0][1] = std::make_shared<Knight>(Color::WHITE);
    board_[0][2] = std::make_shared<Bishop>(Color::WHITE);
    board_[0][3] = std::make_shared<Queen>(Color::WHITE);
    board_[0][4] = std::make_shared<King>(Color::WHITE);
    board_[0][5] = std::make_shared<Bishop>(Color::WHITE);
    board_[0][6] = std::make_shared<Knight>(Color::WHITE);
    board_[0][7] = std::make_shared<Rook>(Color::WHITE);

    for (int i = 0; i < 8; ++i) {
        board_[1][i] = std::make_shared<Pawn>(Color::WHITE);
        board_[6][i] = std::make_shared<Pawn>(Color::BLACK);
    }

    board_[7][0] = std::make_shared<Rook>(Color::BLACK);
    board_[7][1] = std::make_shared<Knight>(Color::BLACK);
    board_[7][2] = std::make_shared<Bishop>(Color::BLACK);
    board_[7][3] = std::make_shared<Queen>(Color::BLACK);
    board_[7][4] = std::make_shared<King>(Color::BLACK);
    board_[7][5] = std::make_shared<Bishop>(Color::BLACK);
    board_[7][6] = std::make_shared<Knight>(Color::BLACK);
    board_[7][7] = std::make_shared<Rook>(Color::BLACK);
}

void Board::display() const {
    std::cout << "   a b c d e f g h\n  -----------------" << std::endl;
    for (int i = 7; i >= 0; --i) {
        std::string line = std::to_string(i + 1) + "| ";
        for (int j = 0; j < 8; ++j) {
            if (board_[i][j] != nullptr) {
                line += board_[i][j]->name();
            } else
                line += '.';
            line += ' ';
        }
        line += '|' + std::to_string(i + 1);
        std::cout << line << std::endl;
    }
    std::cout << "  -----------------\n   a b c d e f g h" << std::endl;
}

void Board::setPieceAt(const Coordinate& coordinate, std::shared_ptr<Piece> piece) {
    board_[coordinate.rank()][coordinate.file()] = std::move(piece);
}

void Board::movePiece(const Coordinate& source, const Coordinate& destination) {
    board_[destination.rank()][destination.file()] = std::move(
        board_[source.rank()][source.file()]);
}

void Board::restart() {
    clear();
    initialize();
}

std::vector<Coordinate> Board::computeCheckers(Color color) const
{
    Coordinate kingSquare = findKing(color);
    Color enemy = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    std::vector<Coordinate> checkers;

    // for (int r = 0; r < 8; ++r)
    //     for (int c = 0; c < 8; ++c) {
    //         const auto& piece = board_[r][c];
    //         if (piece && piece->color() == enemy)
    //             if (pieceAttacksSquare({r, c}, kingSquare))
    //                 checkers.push_back({r, c});
    //     }

    return checkers;
}

Coordinate Board::findKing(Color color) const
{
    for (int r = 0; r < board_.size(); r++) {
        for (int f = 0; f < board_.size(); f++) {
            const auto& piece = board_[r][f];
            if (piece && piece->type() == PieceType::KING && piece->color() == color ) {
                return {r, f};
            }
        }
    }
    throw std::runtime_error("King not found - board state is invalid!");
}

const bool Board::isFriendly(Coordinate &coord, Color color) const
{
    const auto target = getPieceAt(coord);
    if (target) {
        return target->color() == color;;
    }
    return false;
}

const bool Board::isEnemy(Coordinate &coord, Color color) const
{
    const auto target = getPieceAt(coord);
    if (target) {
        return target->color() != color;;
    }
    return false;
}

const bool Board::inBounds(Coordinate &coord) const
{
    return coord.rank() >= 0 && coord.rank() < board_.size()
           && coord.file() >= 0 && coord.file() < board_.size();
}


std::shared_ptr<Piece> Board::getPieceAt(const Coordinate& coordinate) const {
    return board_[coordinate.rank()][coordinate.file()];
}

void Board::clear() {
    board_ = {};
}
