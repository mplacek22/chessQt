#include "move.h"
#include "piece.h"

const std::string Move::toChessNotation(const Move& move) {
    if (move.moveType == MoveType::CASTLE_KINGSIDE)  return "O-O";
    if (move.moveType == MoveType::CASTLE_QUEENSIDE) return "O-O-O";

    std::string notation;
    if (move.movingPiece->type() != PieceType::PAWN) {
        notation += move.movingPiece->name();
    }

    if(move.isAmbiguous) {
        notation += move.source.toChessNotation();
    }

    if (move.capturedPiece) {
        if (move.moveType == MoveType::ENPASSANT) {
            notation += move.source.toChessNotation();
        }
        notation += 'x';
    }

    notation += move.destination.toChessNotation();


    if (move.moveType == MoveType::PROMOTION) {
        notation += '=';
        switch (move.promotionPieceType.value()) {
            case PieceType::QUEEN:  notation += 'Q'; break;
            case PieceType::ROOK:   notation += 'R'; break;
            case PieceType::BISHOP: notation += 'B'; break;
            case PieceType::KNIGHT: notation += 'N'; break;
            default: break;
        }
    }

    switch (move.gameStatus) {
        case GameStatus::CHECK: notation += '+'; break;
        case GameStatus::CHECK_MATE: notation += '#'; break;
        default: break;
    }

    return notation;
}