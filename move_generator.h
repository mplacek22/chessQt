#pragma once

#include "Move.h"
#include "board.h"
#include "game_state.h"


class MoveGenerator
{
public:

    static std::vector<std::shared_ptr<Move>> calculatePseudoLegalMoves(const Board& board, const Coordinate& source)
    {
        std::vector<std::shared_ptr<Move> > moves;
        std::shared_ptr<Piece> piece = board.getPieceAt(source);

        for (const auto& dir : piece->getMoveDirections()) {
            int rank = source.rank();
            int file = source.file();

            do {
                rank += dir[0];
                file += dir[1];

                Coordinate destination {rank, file};

                if (!board.inBounds(destination)) break;

                if (board.isFriendly(destination, piece->color())) break;

                auto move = std::make_shared<Move>(source, destination, piece->color());
                move->movingPiece = piece;
                const auto capturedPiece = board.getPieceAt(destination);
                if (capturedPiece) {
                    move->moveType = MoveType::CAPTURE;
                    move->capturedPiece = capturedPiece;
                }
                else {
                    move->moveType = MoveType::NORMAL;
                }
                moves.push_back(move);

                if (board.isEnemy(destination, piece->color())) break;

            } while (piece->isSliding());
        }

        return moves;
    }

    static std::vector<std::shared_ptr<Move>> calculatePossibleMoves(const Board& board, const Coordinate& source, const GameState& state)
    {
        auto piece = board.getPieceAt(source);
        switch (piece->type()) {
        case PieceType::PAWN:   return calculatePawnMoves(board, source, state);
        case PieceType::KING:   return calculateKingMoves(board, source, state);
        default:                return MoveGenerator::calculatePseudoLegalMoves(board, source); // B, R, Q, K
        }
    }


private:
    static std::vector<std::shared_ptr<Move>> calculatePawnMoves(const Board& board, const Coordinate& source, const GameState& state)
    {
        std::vector<std::shared_ptr<Move>> moves;
        int moveDirection = state.currentPlayer == Color::WHITE ? 1 : -1;
        int promotionRank = state.currentPlayer == Color::WHITE ? board.board().size() - 1 : 0;

        // forward move - 1 square
        int rank = source.rank() + moveDirection;
        int file = source.file();
        Coordinate destination {rank, file};
        const auto movingPiece = board.getPieceAt(source);

        if (board.inBounds(destination) && board.getPieceAt(destination) == nullptr){

            auto move = std::make_shared<Move>(source, destination, state.currentPlayer);
            move->movingPiece = movingPiece;

            // promotion
            move->moveType = rank == promotionRank ? MoveType::PROMOTION : MoveType::NORMAL;

            moves.push_back(std::move(move));
        }

        // forward move - 2 squares
        rank += moveDirection;
        destination = {rank, file};
        if (!movingPiece->hasMoved() && board.inBounds(destination) && board.getPieceAt(destination) == nullptr) {
            auto move = std::make_shared<Move>(source, destination, movingPiece->color());
            move->movingPiece = movingPiece;
            move->moveType = MoveType::PAWN_DOUBLE_NORMAL;
            moves.push_back(std::move(move));
        }

        // capture
        rank = source.rank() + moveDirection;
        for (int f : {-1, 1}) {
            file = source.file() + f;
            destination = {rank, file};
            if (board.inBounds(destination)){
                auto move = std::make_shared<Move>(source, destination, state.currentPlayer);
                move->movingPiece = movingPiece;

                if (board.isEnemy(destination, state.currentPlayer)) {
                    move->moveType = rank == promotionRank ? MoveType::PROMOTION : MoveType::CAPTURE;
                    move->capturedPiece = board.getPieceAt(destination);
                    moves.push_back(std::move(move));
                }
                // en passant
                if (state.lastMove && state.lastMove->moveType == MoveType::PAWN_DOUBLE_NORMAL){
                    int df = state.lastMove.value().destination.file() - source.file();
                    // is captured piece next to moving piece
                    bool next = state.lastMove.value().destination.rank() == source.rank() && (df == 1 || df == -1);
                    // check if the destination of the last move matches the expected captured pawn position
                    bool isCapturableTarget = state.lastMove->destination.file() == destination.file();
                    if (board.getPieceAt(destination) == nullptr && next && isCapturableTarget) {
                        move->moveType = MoveType::ENPASSANT;
                        move->capturedPiece = board.getPieceAt({source.rank(), file});
                        moves.push_back(std::move(move));
                    }
                }

            }
        }
        return moves;
    }

    static std::vector<std::shared_ptr<Move>> calculateKingMoves(const Board& board, const Coordinate& source, const GameState& state)
    {
        auto moves =  MoveGenerator::calculatePseudoLegalMoves(board, source);
        auto dangerSquares = computeKingDangerSquares(board, source);

        // Filter out moves to king danger squares
        std::erase_if(moves, [&](const auto& m) {
            return std::ranges::find(dangerSquares, m->destination) != dangerSquares.end();
        });

        return moves;
    }

    static std::vector<Coordinate> computeKingDangerSquares(const Board& board,const Coordinate& kingPosition)
    {
        Color color = board.getPieceAt(kingPosition)->color();
        std::array<std::array<std::shared_ptr<Piece>, 8>, 8> boardWithoutKing = board.board();
        boardWithoutKing[kingPosition.rank()][kingPosition.file()] = nullptr;

        std::vector<Coordinate> danger;
        for (int r = 0; r < boardWithoutKing.size(); r++) {
            for (int f = 0; f < boardWithoutKing.size(); f++) {
                const auto piece = boardWithoutKing[r][f];
                if (piece && piece->color() != color) {
                    for (const auto&  m :  MoveGenerator::calculatePseudoLegalMoves(board, {r, f})) {
                        danger.push_back(m->destination);
                    }
                }
            }
        }
        return danger;
    }

};