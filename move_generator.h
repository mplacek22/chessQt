#pragma once

#include "Move.h"
#include "board.h"
#include "game_state.h"
#include <algorithm>

class MoveGenerator
{
public:

    // All moves ignoring whether they leave the king in check
    static std::vector<Move> calculatePseudoLegalMoves(const Board& board, const Coordinate& source, const GameState& state)
    {
        auto piece = board.getPieceAt(source);
        if (!piece) return {};
        switch (piece->type()) {
            case PieceType::PAWN:   return calculatePawnMoves(board, source, state);
            default:                return calculateRegularMoves(board, source); // B, R, Q, N, K
        }
    }

    // Filters pseudo-legal moves to only those that are fully legal
    static std::vector<Move> calculatePossibleMoves(const Board& board, const Coordinate& source, const GameState& state)
    {
        auto piece = board.getPieceAt(source);
        if (!piece) return {};

        // in double check only the king can move
        if(state.gameStatus == GameStatus::DOUBLE_CHECK && board.getPieceAt(source)->type() != PieceType::KING) {
            return {};
        }

        std::vector<Move> pseudo;
        switch (piece->type()) {
            case PieceType::PAWN:
                pseudo = calculatePawnMoves(board, source, state);
                break;
            case PieceType::KING:
                pseudo = calculateKingMoves(board, source, state);
                break;
            default:
                pseudo = calculateRegularMoves(board, source); // B, R, Q, N
                break;
        }

        std::vector<Move> legal;
        legal.reserve(pseudo.size());

        for (const auto& move : pseudo) {
            if (!leavesKingInCheck(move, board, state))
                legal.push_back(move);
        }

        return legal;
    }

    static std::vector<Coordinate> computeCheckers(const Board& board, const GameState& gameState)
    {
        Color enemy = (gameState.currentPlayer == Color::WHITE) ? Color::BLACK : Color::WHITE;

        Coordinate kingPosition = board.findKing(enemy);

        std::vector<Coordinate> checkers;

        for (int r = 0; r < Board::BOARD_SIZE; ++r) {
            for (int f = 0; f < Board::BOARD_SIZE; ++f) {
                Coordinate source {r, f};
                auto piece = board.getPieceAt(source);
                if (!piece || piece->color() != gameState.currentPlayer) continue;

                auto moves = calculatePseudoLegalMoves(board, source, gameState);
                for (const auto& move : moves) {
                    if (move.destination == kingPosition) {
                        checkers.push_back(source);
                        if (checkers.size() == 2) {
                            return checkers;
                        }
                        break;
                    }
                }
            }
        }
        return checkers;
    }

    static bool canPlayerMove(const Board& board, const GameState& gameState, const Color color) {
        std::vector<Coordinate> pieces_coords;

        for (int r = 0; r < Board::BOARD_SIZE; ++r) {
            for (int f = 0; f < Board::BOARD_SIZE; ++f) {
                Coordinate coord {r, f};
                auto piece = board.getPieceAt(coord);
                if(piece && piece->color() == color) {
                    pieces_coords.push_back(coord);
                }
            }
        }
        for (const Coordinate& source : pieces_coords) {
            // any move possible
            if (!calculatePossibleMoves(board, source, gameState).empty()) {
                return true;
            }
        }
        return false;
    }

    static bool canCastleKingSide(const Board& board, const GameState& state) {
        return canCastle(board, state, /*kingSide=*/true);
    }

    static bool canCastleQueenSide(const Board& board, const GameState& state) {
        return canCastle(board, state, /*kingSide=*/false);
    }

private:
    static std::vector<Move> calculatePawnMoves(const Board& board, const Coordinate& source, const GameState& state)
    {
        std::vector<Move> moves;
        int moveDirection = state.currentPlayer == Color::WHITE ? 1 : -1;
        int promotionRank = state.currentPlayer == Color::WHITE ? Board::BOARD_SIZE - 1 : 0;

        // forward move - 1 square
        int rank = source.rank + moveDirection;
        int file = source.file;
        Coordinate destination {rank, file};
        const auto movingPiece = board.getPieceAt(source);

        if (board.inBounds(destination) && board.getPieceAt(destination) == nullptr){

            Move move{source, destination, state.currentPlayer};
            move.movingPieceType = movingPiece->type();

            // promotion
            move.moveType = rank == promotionRank ? MoveType::PROMOTION : MoveType::NORMAL;

            moves.push_back(std::move(move));
        }

        // forward move - 2 squares
        rank += moveDirection;
        destination = {rank, file};
        if (!movingPiece->hasMoved() && board.inBounds(destination) && board.getPieceAt(destination) == nullptr) {
            Move move{source, destination, movingPiece->color()};
            move.movingPieceType = movingPiece->type();
            move.moveType = MoveType::PAWN_DOUBLE_NORMAL;
            moves.push_back(std::move(move));
        }

        // capture
        rank = source.rank + moveDirection;
        for (int f : {-1, 1}) {
            file = source.file + f;
            destination = {rank, file};
            if (board.inBounds(destination)){
                Move move {source, destination, state.currentPlayer};
                move.movingPieceType = movingPiece->type();
                move.isCapture = true;

                if (board.isEnemy(destination, state.currentPlayer)) {
                    move.moveType = rank == promotionRank ? MoveType::PROMOTION : MoveType::CAPTURE;
                    moves.push_back(std::move(move));
                }
                // en passant
                if (state.lastMove && state.lastMove->moveType == MoveType::PAWN_DOUBLE_NORMAL){
                    int df = state.lastMove.value().destination.file - source.file;
                    // is captured piece next to moving piece
                    bool next = state.lastMove.value().destination.rank == source.rank && (df == 1 || df == -1);
                    // check if the destination of the last move matches the expected captured pawn position
                    bool isCapturableTarget = state.lastMove->destination.file == destination.file;
                    if (board.getPieceAt(destination) == nullptr && next && isCapturableTarget) {
                        move.moveType = MoveType::ENPASSANT;
                        moves.push_back(std::move(move));
                    }
                }

            }
        }
        return moves;
    }

    static std::vector<Move> calculateKingMoves(const Board& board, const Coordinate& source, const GameState& state)
    {
        auto moves = calculateRegularMoves(board, source);
        auto dangerSquares = computeKingDangerSquares(board, source, state);

        // Filter out moves to king danger squares
        std::erase_if(moves, [&](const auto& m) {
            return std::ranges::find(dangerSquares, m.destination) != dangerSquares.end();
        });

        if(canCastleKingSide(board, state)) {
            Move move {source, Coordinate{source.rank, 6}, state.currentPlayer};
            move.moveType = MoveType::CASTLE_KINGSIDE;
            move.movingPieceType = board.getPieceAt(source)->type();
            moves.push_back(move);
        }
        if(canCastleQueenSide(board, state)) {
            Move move {source, Coordinate{source.rank, 2}, state.currentPlayer};
            move.moveType = MoveType::CASTLE_QUEENSIDE;
            move.movingPieceType = board.getPieceAt(source)->type();
            moves.push_back(move);
        }

        return moves;
    }

    static std::vector<Coordinate> computeKingDangerSquares(const Board& board,const Coordinate& kingPosition, const GameState& gameState)
    {
        Color color = board.getPieceAt(kingPosition)->color();
        auto boardWithoutKing = board.board();
        boardWithoutKing.set(kingPosition, nullptr);

        std::vector<Coordinate> danger;
        for (int r = 0; r < Board::BOARD_SIZE; r++) {
            for (int f = 0; f < Board::BOARD_SIZE; f++) {
                const auto piece = boardWithoutKing.at({r, f});
                if (piece && piece->color() != color) {
                    for (const auto&  m : MoveGenerator::calculatePseudoLegalMoves(board, {r, f}, gameState)) {
                        danger.push_back(m.destination);
                    }
                }
            }
        }
        return danger;
    }

    static bool canCastle(const Board& board, const GameState& state, bool kingSide) {
        // no check
        if(state.gameStatus == GameStatus::SINGLE_CHECK || state.gameStatus == GameStatus::DOUBLE_CHECK){
            return false;
        }
        // king hasnt moved
        Coordinate kingStartPosition(state.currentPlayer == Color::WHITE ? 0 : 7, 4);
        Coordinate kingPosition = board.findKing(state.currentPlayer);
        auto king = board.getPieceAt(kingPosition);
        if(kingPosition != kingStartPosition || king->hasMoved()) {
            return false;
        }
        // rook hasnt moved
        int rookFileOffset = kingSide ? 3 : -4;
        Coordinate rookStartPosition = {kingPosition.rank, kingPosition.file + rookFileOffset};
        auto rook = board.getPieceAt(rookStartPosition);
        if (!rook || rook->type() != PieceType::ROOK || rook->hasMoved()) {
            return false;
        }
        // clear path
        if (!board.isPathClear(kingPosition, rookStartPosition)){
            return false;
        }

        // path squares cant be attacked
        Color attackerColor = state.currentPlayer == Color::WHITE ? Color::BLACK : Color::WHITE;
        int direction = kingSide ? 1 : -1;
        for (int step = 0; step <= 2; ++step) {
            Coordinate square = {kingPosition.rank, kingPosition.file + direction * step};
            if (isSquareAttackedBy(board, square, attackerColor, state)) {
                return false;
            }
        }

        return true;
    }

    static bool isSquareAttackedBy(const Board& board, const Coordinate& target, Color attackerColor, const GameState& gameState)
    {
        GameState nextMoveState{attackerColor, gameState.gameStatus};

        for (int r = 0; r < Board::BOARD_SIZE; ++r) {
            for (int f = 0; f < Board::BOARD_SIZE; ++f) {
                Coordinate source = {r, f};
                auto piece = board.getPieceAt(source);
                if (!piece || piece->color() != attackerColor) continue;

                auto moves = calculatePseudoLegalMoves(board, source, nextMoveState);

                for (const auto& move : moves) {
                    if (move.destination == target && move.isCapture)
                        return true;
                }
            }
        }
        return false;
    }

    static std::vector<Move> calculateRegularMoves(const Board& board, const Coordinate& source) {
        std::shared_ptr<Piece> piece = board.getPieceAt(source);
        if (!piece) return {};

        std::vector<Move> moves;

        for (const auto& dir : piece->getMoveDirections()) {
            int rank = source.rank;
            int file = source.file;

            do {
                rank += dir[0];
                file += dir[1];

                Coordinate destination {rank, file};

                if (!board.inBounds(destination)) break;

                if (board.isFriendly(destination, piece->color())) break;

                Move move {source, destination, piece->color()};
                move.movingPieceType = piece->type();
                const auto capturedPiece = board.getPieceAt(destination);
                if (capturedPiece) {
                    move.moveType = MoveType::CAPTURE;
                    move.isCapture = true;
                }
                else {
                    move.moveType = MoveType::NORMAL;
                }
                moves.push_back(move);

                if (board.isEnemy(destination, piece->color())) break;

            } while (piece->isSliding());
        }

        return moves;
    }

    static bool leavesKingInCheck(const Move& move, const Board& board, const GameState& state)
    {
        Board copy = board;
        auto movingPiece = copy.getPieceAt(move.source);
        copy.movePiece(move.source, move.destination);

        //todo: handle en passant
        auto kingSquare = copy.findKing(movingPiece->color());
        Color enemy = (movingPiece->color() == Color::WHITE) ? Color::BLACK : Color::WHITE;
        return isSquareAttackedBy(copy, kingSquare, enemy, state);
    }
};