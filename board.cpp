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
    initialize();
}


std::shared_ptr<Piece> Board::getPieceAt(const Coordinate& coordinate) const {
    return board_[coordinate.rank()][coordinate.file()];
}
