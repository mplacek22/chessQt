#include "Bishop.h"
#include "King.h"
#include "Pawn.h"
#include "Queen.h"
#include "Rook.h"
#include "Knight.h"

Piece::Piece(const Color color) {
    color_ = color;
}

std::shared_ptr<Piece> Piece::create(Color color, const PieceType pieceType) {
    switch (pieceType) {
    case PieceType::PAWN:
        return std::make_shared<Pawn>(color);
    case PieceType::KNIGHT:
        return std::make_shared<Knight>(color);
    case PieceType::BISHOP:
        return std::make_shared<Bishop>(color);
    case PieceType::ROOK:
        return std::make_shared<Rook>(color);
    case PieceType::KING:
        return std::make_shared<King>(color);
    case PieceType::QUEEN:
        return std::make_shared<Queen>(color);
    default:
        return nullptr;
    }
}