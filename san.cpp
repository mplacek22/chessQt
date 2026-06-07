#include "san.h"

#include <string>

#include "InvalidSANException.h"
#include "move_generator.h"

namespace {
    char rankSAN(const int rank) {
        if (rank < 0 || rank >= Board::BOARD_SIZE) {
            throw InvalidSanException("Invalid rank: " + std::to_string(rank));
        }
        return static_cast<char>('1' + rank);
    }

    char fileSAN(const int file) {
        if (file < 0 || file >= Board::BOARD_SIZE) {
            throw InvalidSanException("Invalid rank: " + std::to_string(file));
        }
        return static_cast<char>('a' + file);
    }

    std::string coordinateToSAN(const Coordinate &coordinate) {
        return {fileSAN(coordinate.file), rankSAN(coordinate.rank)};
    }

    char pieceTypeToChar(const PieceType pieceType) {
        switch (pieceType) {
            case PieceType::BISHOP: return 'B';
            case PieceType::KING: return 'K';
            case PieceType::KNIGHT: return 'N';
            case PieceType::ROOK: return 'R';
            case PieceType::QUEEN: return 'Q';
            default: return '\0';
        }
    }
}


std::string san::toSAN(const Move &move) {
    if (move.moveType == MoveType::CASTLE_KINGSIDE) return "O-O";
    if (move.moveType == MoveType::CASTLE_QUEENSIDE) return "O-O-O";

    std::string notation;
    if (move.movingPieceType != PieceType::PAWN) {
        notation += pieceTypeToChar(move.movingPieceType);
    }

    // if(move.isAmbiguous) {
    //     notation += coordinateToSAN(move.source);
    // }
    notation += coordinateToSAN(move.source);


    if (move.isCapture) {
        notation += 'x';
    }

    notation += coordinateToSAN(move.destination);


    if (move.moveType == MoveType::PROMOTION) {
        notation += '=';
        switch (move.promotionPieceType.value()) {
            case PieceType::QUEEN: notation += 'Q';
                break;
            case PieceType::ROOK: notation += 'R';
                break;
            case PieceType::BISHOP: notation += 'B';
                break;
            case PieceType::KNIGHT: notation += 'N';
                break;
            default: break;
        }
    }

    switch (move.gameStatusAfterMove) {
        case GameStatus::SINGLE_CHECK:
        case GameStatus::DOUBLE_CHECK:
            notation += '+';
            break;
        case GameStatus::CHECK_MATE:
            notation += '#';
            break;
        default: break;
    }

    return notation;
}

Coordinate san::coordinateFromString(const std::string &raw) {
    if (raw.size() < 2) {
        throw InvalidSanException("Coordinate too short: " + raw);
    }
    if (raw[0] < 'a' || raw[0] > 'h' ||
        raw[1] < '1' || raw[1] > '8') {
        throw InvalidSanException("File must be between 'a' and 'h' and rank between '1' and '8'");
    }
    int file = raw[0] - 'a';
    int rank = raw[1] - '1';
    return {rank, file};
}

PieceType san::charToPieceType(char c) {
    switch (std::toupper(c)) {
        case 'B': return PieceType::BISHOP;
        case 'K': return PieceType::KING;
        case 'N': return PieceType::KNIGHT;
        case 'P': return PieceType::PAWN;
        case 'Q': return PieceType::QUEEN;
        case 'R': return PieceType::ROOK;
        default: throw InvalidSanException(std::string("Unknown piece char: ") + c);
    }
}
