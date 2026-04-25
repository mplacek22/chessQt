#include "Bishop.h"
#include "King.h"
#include "Pawn.h"
#include "Queen.h"
#include "Rook.h"
#include "Knight.h"
#include "Move.h"

#include <stdexcept>

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

Coordinate Piece::findKing(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8> &board) const
{
    for (int r = 0; r < board.size(); r++) {
        for (int f = 0; f < board.size(); f++) {
            const auto& piece = board[r][f];
            if (piece && piece->type() == PieceType::KING && piece->color() == color_ ) {
                return {r, f};
            }
        }
    }
    throw std::runtime_error("King not found on board");
}

std::vector<Coordinate> Piece::computeCheckers(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8> &board) const
{
    std::vector<Coordinate> checkers;
    auto kingPosition = findKing(board);

    // for (int r = 0; r < board.size(); r++) {
    //     for (int f = 0; f < 8; f++) {
    //         const auto piece = board[r][f];
    //         if (!piece || piece->color() != color_) continue;

    //         for (const auto& m : piece->calculatePseudoLegalMoves(board, {r, f})) {
    //             if (m->destination == kingPosition) {
    //                 checkers.push_back({r, f});
    //                 break;
    //             }
    //         }
    //     }
    // }
    return checkers;
}

std::vector<std::shared_ptr<Move> > Piece::calculatePseudoLegalMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8> &board, const Coordinate &source)
{
    std::vector<std::shared_ptr<Move> > moves;

    for (const auto& dir : getMoveDirections()) {
        int rank = source.rank();
        int file = source.file();

        do {
            rank += dir[0];
            file += dir[1];

            if (!inBounds(rank, file)) break;

            Coordinate destination {rank, file};

            if (isFriendly(board, destination)) break;

            auto move = std::make_shared<Move>(source, destination, color_);
            move->movingPiece = board[source.rank()][source.file()];
            const auto& capturedPiece = board[rank][file];
            if (capturedPiece) {
                move->moveType = MoveType::CAPTURE;
                move->capturedPiece = capturedPiece;
            }
            else {
                move->moveType = MoveType::NORMAL;
            }
            moves.push_back(move);

            if (isEnemy(board, destination)) break;

        } while (isSliding());
    }

    return moves;
}

const bool Piece::isEnemy(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8> &board, Coordinate &coord)
{
    const auto& target = board[coord.rank()][coord.file()];
    if (target) {
        return target->color() != color_;;
    }
    return false;
}

std::vector<std::shared_ptr<Move> > Piece::calculatePossibleMoves(
    const std::array<std::array<std::shared_ptr<Piece>, 8>, 8> &board,
    const Coordinate &source,
    GameStatus gameStatus,
    std::optional<Move> lastMove
) {
    std::vector<std::shared_ptr<Move> > moves;

    // in double check only the king can move
    if (gameStatus == GameStatus::DOUBLE_CHECK && type_ != PieceType::KING) {
        return moves;
    }

    moves = calculatePseudoLegalMoves(board, source);

    return moves;
}

const bool Piece::isFriendly(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board, Coordinate &coord)
{
    const auto& target = board[coord.rank()][coord.file()];
    if (target) {
        return target->color() == color_;;
    }
    return false;
}