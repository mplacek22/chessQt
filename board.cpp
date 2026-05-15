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
    board_.set(0, 0, std::make_shared<Rook>(Color::WHITE));
    board_.set(0, 1, std::make_shared<Knight>(Color::WHITE));
    board_.set(0, 2, std::make_shared<Bishop>(Color::WHITE));
    board_.set(0, 3, std::make_shared<Queen>(Color::WHITE));
    board_.set(0, 4, std::make_shared<King>(Color::WHITE));
    board_.set(0, 5, std::make_shared<Bishop>(Color::WHITE));
    board_.set(0, 6, std::make_shared<Knight>(Color::WHITE));
    board_.set(0, 7, std::make_shared<Rook>(Color::WHITE));

    for (int i = 0; i < 8; ++i) {
        board_.set(1, i, std::make_shared<Pawn>(Color::WHITE));
        board_.set(6, i, std::make_shared<Pawn>(Color::BLACK));
    }

    board_.set(7, 0, std::make_shared<Rook>(Color::BLACK));
    board_.set(7, 1, std::make_shared<Knight>(Color::BLACK));
    board_.set(7, 2, std::make_shared<Bishop>(Color::BLACK));
    board_.set(7, 3, std::make_shared<Queen>(Color::BLACK));
    board_.set(7, 4, std::make_shared<King>(Color::BLACK));
    board_.set(7, 5, std::make_shared<Bishop>(Color::BLACK));
    board_.set(7, 6, std::make_shared<Knight>(Color::BLACK));
    board_.set(7, 7, std::make_shared<Rook>(Color::BLACK));

    // // double check
    // // White pieces
    // board_.set(0, 4, std::make_shared<King>(Color::WHITE));
    // board_.set(4, 4, std::make_shared<Rook>(Color::WHITE));   // e5 – checks Black King on e8 along e-file
    // board_.set(4, 1, std::make_shared<Bishop>(Color::WHITE)); // b5 – checks Black King on e8 via diagonal

    // // Black pieces
    // board_.set(7, 4, std::make_shared<King>(Color::BLACK));   // e8 – in double check
    // board_.set(7, 0, std::make_shared<Rook>(Color::BLACK));   // a8
    // board_.set(7, 7, std::make_shared<Rook>(Color::BLACK));   // h8

    // // almost check
    // // White pieces
    // // board_.set(0, 4, std::make_shared<King>(Color::WHITE));
    // // board_.set(0, 0, std::make_shared<Rook>(Color::WHITE));    // Will give check on e-file
    // // board_.set(2, 6, std::make_shared<Bishop>(Color::WHITE));  // Will give check on diagonal

    // // Black pieces
    // // board_.set(7, 4, std::make_shared<King>(Color::BLACK));    // Black King on e8
    // // board_.set(7, 0, std::make_shared<Rook>(Color::BLACK));    // Blocker removed to expose check
    // // board_.set(7, 7, std::make_shared<Rook>(Color::BLACK));

    // // pinned pieces
    // // White pieces
    // // board_.set(0, 4, std::make_shared<King>(Color::WHITE));     // e1
    // // board_.set(1, 4, std::make_shared<Knight>(Color::WHITE));   // e2 – pinned piece

    // // Black pieces
    // // board_.set(7, 4, std::make_shared<Rook>(Color::BLACK));     // e8 – creates the pin
    // // board_.set(6, 0, std::make_shared<King>(Color::BLACK));     // a7 – arbitrary safe placement

    // // pinned pieces 2
    // // White pieces
    // // board_.set(0, 4, std::make_shared<King>(Color::WHITE));     // e1
    // // board_.set(1, 4, std::make_shared<Bishop>(Color::WHITE));   // e2 – pinned piece

    // // Black pieces
    // // board_.set(7, 4, std::make_shared<Rook>(Color::BLACK));     // e8 – creates the pin
    // // board_.set(6, 0, std::make_shared<King>(Color::BLACK));     // a7 – arbitrary safe placement

    // // almost checkmate
    // // White king
    // // board_.set(5, 5, std::make_shared<King>(Color::WHITE));   // f6

    // // White queen
    // // board_.set(5, 6, std::make_shared<Queen>(Color::WHITE));  // g6

    // // Black king
    // // board_.set(7, 7, std::make_shared<King>(Color::BLACK));   // h8

    // // castling
    // // --- WHITE (rank 0) ---
    // board_.set(0, 0, std::make_shared<Rook>(Color::WHITE));     // a1 - unmoved, queen-side rook
    // board_.set(0, 1, std::make_shared<Knight>(Color::WHITE));   // b1 - BLOCKS queen-side castle
    // // c1, d1 empty
    // board_.set(0, 4, std::make_shared<King>(Color::WHITE));     // e1 - unmoved
    // // f1, g1 empty
    // board_.set(0, 7, std::make_shared<Rook>(Color::WHITE));     // h1 - unmoved, king-side rook

    // // --- BLACK (rank 7) ---
    // board_.set(7, 0, std::make_shared<Rook>(Color::BLACK));     // a8 - unmoved, queen-side rook
    // // b8, c8, d8 empty
    // board_.set(7, 4, std::make_shared<King>(Color::BLACK));     // e8 - unmoved
    // board_.set(7, 5, std::make_shared<Bishop>(Color::BLACK));   // f8 - BLOCKS king-side castle
    // board_.set(7, 7, std::make_shared<Rook>(Color::BLACK));     // h8 - unmoved

    // // --- A white rook on e5 gives check to black king on e8, preventing king-side castle ---
    // board_.set(4, 4, std::make_shared<Rook>(Color::WHITE));     // e5 - attacks e8, puts black in SINGLE_CHECK
}

// void Board::display() const {
//     std::cout << "   a b c d e f g h\n  -----------------" << std::endl;
//     for (int i = 7; i >= 0; --i) {
//         std::string line = std::to_string(i + 1) + "| ";
//         for (int j = 0; j < 8; ++j) {
//             if (board_[i][j] != nullptr) {
//                 line += board_[i][j]->name();
//             } else
//                 line += '.';
//             line += ' ';
//         }
//         line += '|' + std::to_string(i + 1);
//         std::cout << line << std::endl;
//     }
//     std::cout << "  -----------------\n   a b c d e f g h" << std::endl;
// }

void Board::setPieceAt(const Coordinate& coordinate, std::shared_ptr<Piece> piece) {
    board_.set(coordinate, piece);
}

void Board::movePiece(const Coordinate& source, const Coordinate& destination) {
    board_.move(source, destination);
}

void Board::restart() {
    clear();
    initialize();
}

Coordinate Board::findKing(Color color) const
{
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int f = 0; f < BOARD_SIZE; f++) {
            const auto& piece = board_.at(r, f);
            if (piece && piece->type() == PieceType::KING && piece->color() == color ) {
                return {r, f};
            }
        }
    }
    throw std::runtime_error("King not found - board state is invalid!");
}

bool Board::isFriendly(Coordinate &coord, Color color) const
{
    const auto target = getPieceAt(coord);
    if (target) {
        return target->color() == color;;
    }
    return false;
}

bool Board::isEnemy(Coordinate &coord, Color color) const
{
    const auto target = getPieceAt(coord);
    if (target) {
        return target->color() != color;;
    }
    return false;
}

bool Board::inBounds(const Coordinate &coord) const
{
    return board_.inBounds(coord);
}

bool Board::isPathClear(const Coordinate& source, const Coordinate& destination) const
{
    const int dRank = destination.rank() - source.rank();
    const int dFile = destination.file() - source.file();

    const int stepRank = dRank == 0 ? 0 : dRank > 0 ? 1 : -1;
    const int stepFile = dFile == 0 ? 0 : dFile > 0 ? 1 : -1;

    int rank = source.rank() + stepRank;
    int file = source.file() + stepFile;

    while (rank != destination.rank() || file != destination.file()) {
        if (getPieceAt({rank, file}) != nullptr) {
            return false;
        }
        rank += stepRank;
        file += stepFile;
    }
    return true;
}

bool Board::isLightSquare(int rank, int file) const
{
    return (rank + file) % 2 == 0;
}

std::shared_ptr<Piece> Board::getPieceAt(const Coordinate& coordinate) const {
    return board_.at(coordinate);
}

void Board::clear() {
    board_ = {};
}
