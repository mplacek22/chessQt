#include "Bishop.h"
#include "King.h"
#include "Pawn.h"
#include "Queen.h"
#include "Rook.h"
#include "Knight.h"

PieceType Piece::type() const { return type_; }

Color Piece::color() const { return color_; }

bool Piece::hasMoved() const { return hasMoved_; }

void Piece::setHasMoved(const bool hasMoved) { hasMoved_ = hasMoved; }

std::unique_ptr<Piece> Piece::create(Color color, const PieceType pieceType) {
    switch (pieceType) {
    case PieceType::PAWN:
        return std::make_unique<Pawn>(color);
    case PieceType::KNIGHT:
        return std::make_unique<Knight>(color);
    case PieceType::BISHOP:
        return std::make_unique<Bishop>(color);
    case PieceType::ROOK:
        return std::make_unique<Rook>(color);
    case PieceType::KING:
        return std::make_unique<King>(color);
    case PieceType::QUEEN:
        return std::make_unique<Queen>(color);
    default:
        return nullptr;
    }
}