#pragma once

#include "Move.h"
#include "board.h"
#include "game_state.h"
#include <algorithm>
#include <vector>

#include "pawn.h"

/**
 * @brief Chess move generator - calculates all legal moves. Applies chess move rules.
 *
 */
class MoveGenerator
{
public:

    /**
     * @brief Generate all fully legal moves for the piece at @p source square.
     *
     * 1. Returns empty if @p source square is empty.
     * 2. Returns an empty list for any non-king piece if gameStatus is double check.
     * 3. Calculates pseudo-legal moves. Delegates the calculation based on pieceType.
     * 4. Filters out moves that live king in check.
     *
     * @param source Source square of the piece to generate moves for.
     * @param state Current game state.
     * @return List of fully legal moves from @p source square.
     */
    static std::vector<Move> calculatePossibleMoves(const Coordinate& source, const GameState& state)
    {
        const auto piece = state.board.at(source);
        if (!piece) return {};

        // in double check only the king can move
        if(state.gameStatus == GameStatus::DOUBLE_CHECK && state.board.at(source)->type() != PieceType::KING) {
            return {};
        }

        std::vector<Move> pseudo;
        switch (piece->type()) {
            case PieceType::PAWN:
                pseudo = calculatePawnMoves(source, state);
                break;
            case PieceType::KING:
                pseudo = calculateKingMoves(source, state);
                break;
            default:
                pseudo = calculateRegularMoves(state.board, source); // B, R, Q, N
                break;
        }

        std::vector<Move> legal;
        legal.reserve(pseudo.size());

        for (const auto& move : pseudo) {
            if (!leavesKingInCheck(move, state))
                legal.push_back(move);
        }

        return legal;
    }

    /**
     * @brief Find all squares, where pieces are currently giving check to the enemy king.
     *
     * Iterates over the board. Tests whether any of the @p gameState.currentPlayer
     * pieces can perform a pseudo legal move that reaches the enemy king's square.
     *
     * Stops early once two checkers are found
     * (double check is the maximum possible in a legal chess position).
     *
     * @param gameState Current game state (@c currentPlayer is the attacking side).
     * @return Coordinates of the checking pieces (0, 1, or 2 entries).
     */
    static std::vector<Coordinate> computeCheckers(const GameState& gameState)
    {
        const Color enemy = gameState.currentPlayer == Color::WHITE ? Color::BLACK : Color::WHITE;

        const Coordinate kingPosition = gameState.board.findKing(enemy);

        std::vector<Coordinate> checkers;

        for (int r = 0; r < Board::BOARD_SIZE; ++r) {
            for (int f = 0; f < Board::BOARD_SIZE; ++f) {
                Coordinate source {r, f};
                const auto piece = gameState.board.at(source);
                if (!piece || piece->color() != gameState.currentPlayer) continue;

                auto moves = calculatePseudoLegalMoves(source, gameState);
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


    /**
     * @brief Determines whether a player can perform any legal move.
     *
     * It is used to detect checkmate and stalemate
     * If player is in check and this returns @c false it is checkmate.
     * If not in check and this returns @c false it is stalemate.
     *
     * @param gameState Current game state.
     * @param player The player who is being tested.
     * @return @c true if @p player has at least one legal move.
     * */
    static bool canPlayerMove(const GameState& gameState, const Color player) {
        std::vector<Coordinate> pieces_coords;

        for (int r = 0; r < Board::BOARD_SIZE; ++r) {
            for (int f = 0; f < Board::BOARD_SIZE; ++f) {
                Coordinate coord {r, f};
                const auto piece = gameState.board.at(coord);
                if(piece && piece->color() == player) {
                    pieces_coords.push_back(coord);
                }
            }
        }
        return std::ranges::any_of(pieces_coords,
             [&](const auto& source) {
                 return !calculatePossibleMoves(source, gameState).empty();
             }
         );
    }

    /**
     * @brief Determines whether @c state.currentPlayer can castle king side.
     *
     * @param state Current game state.
     * @return @c true if king side castling is currently legal.
     */
    static bool canCastleKingSide(const GameState& state) {
        return canCastle(state, true);
    }

    /**
     * @brief Determines whether @c state.currentPlayer can castle queen side.
     *
     * @param state Current game state.
     * @return @c true if queen side castling is currently legal.
     */
    static bool canCastleQueenSide(const GameState& state) {
        return canCastle(state, false);
    }

private:
    // All moves ignoring whether they leave the king in check
    static std::vector<Move> calculatePseudoLegalMoves(const Coordinate& source, const GameState& state)
    {
        const auto piece = state.board.at(source);
        if (!piece) return {};
        switch (piece->type()) {
            case PieceType::PAWN:   return calculatePawnMoves(source, state);
            default:                return calculateRegularMoves(state.board, source); // B, R, Q, N, K
        }
    }

    static std::vector<Move> calculatePawnMoves(const Coordinate& source, const GameState& state)
    {
        std::vector<Move> moves;
        const auto movingPiece = dynamic_cast<Pawn*>(state.board.at(source));
        const int moveDirection = movingPiece->getNormalMoveDirections().front();
        const int promotionRank = state.currentPlayer == Color::WHITE ? Board::BOARD_SIZE - 1 : 0;

        // forward move - 1 square
        int rank = source.rank + moveDirection;
        int file = source.file;
        Coordinate destination {rank, file};
        const bool singlePushClear = Board::inBounds(destination) && state.board.at(destination) == nullptr;

        if (singlePushClear){

            Move move{source, destination, state.currentPlayer};
            move.movingPieceType = movingPiece->type();

            // promotion
            move.moveType = rank == promotionRank ? MoveType::PROMOTION : MoveType::NORMAL;

            moves.push_back(move);
        }

        // forward move - 2 squares
        rank += moveDirection;
        destination = {rank, file};
        if (!movingPiece->hasMoved() && singlePushClear && Board::inBounds(destination) && state.board.at(destination) == nullptr) {
            Move move{source, destination, movingPiece->color()};
            move.movingPieceType = movingPiece->type();
            move.moveType = MoveType::PAWN_DOUBLE_NORMAL;
            moves.push_back(move);
        }

        // capture
        rank = source.rank + moveDirection;
        for (const int f : {-1, 1}) {
            file = source.file + f;
            destination = {rank, file};
            if (Board::inBounds(destination)){
                Move move {source, destination, state.currentPlayer};
                move.movingPieceType = movingPiece->type();
                move.isCapture = true;

                if (state.board.isEnemy(destination, state.currentPlayer)) {
                    move.moveType = rank == promotionRank ? MoveType::PROMOTION : MoveType::CAPTURE;
                    moves.push_back(move);
                }
                // en passant
                if (state.lastMove && state.lastMove->moveType == MoveType::PAWN_DOUBLE_NORMAL){
                    const int df = state.lastMove.value().destination.file - source.file;
                    // is captured piece next to moving piece
                    const bool next = state.lastMove.value().destination.rank == source.rank && (df == 1 || df == -1);
                    // check if the destination of the last move matches the expected captured pawn position
                    const bool isCapturableTarget = state.lastMove->destination.file == destination.file;
                    if (state.board.at(destination) == nullptr && next && isCapturableTarget) {
                        move.moveType = MoveType::ENPASSANT;
                        moves.push_back(move);
                    }
                }

            }
        }
        return moves;
    }

    static std::vector<Move> calculateKingMoves(const Coordinate& source, const GameState& state)
    {
        auto moves = calculateRegularMoves(state.board, source);
        auto dangerSquares = computeKingDangerSquares(source, state);

        // Filter out moves to king danger squares
        std::erase_if(moves, [&](const auto& m) {
            return std::ranges::find(dangerSquares, m.destination) != dangerSquares.end();
        });

        if(canCastleKingSide(state)) {
            Move move {source, Coordinate{source.rank, 6}, state.currentPlayer};
            move.moveType = MoveType::CASTLE_KINGSIDE;
            move.movingPieceType = state.board.at(source)->type();
            moves.push_back(move);
        }
        if(canCastleQueenSide(state)) {
            Move move {source, Coordinate{source.rank, 2}, state.currentPlayer};
            move.moveType = MoveType::CASTLE_QUEENSIDE;
            move.movingPieceType = state.board.at(source)->type();
            moves.push_back(move);
        }

        return moves;
    }

    static std::vector<Coordinate> computeKingDangerSquares(const Coordinate& kingPosition, const GameState& gameState)
    {
        const Color color = gameState.board.at(kingPosition)->color();
        auto boardWithoutKing = gameState.board.clone();
        boardWithoutKing.set(kingPosition, nullptr);

        std::vector<Coordinate> danger;
        for (int r = 0; r < Board::BOARD_SIZE; r++) {
            for (int f = 0; f < Board::BOARD_SIZE; f++) {
                const auto piece = boardWithoutKing.at({r, f});
                if (piece && piece->color() != color) {
                    GameState tmpState = {gameState.currentPlayer, gameState.gameStatus, boardWithoutKing, gameState.lastMove};
                    for (const auto&  m : MoveGenerator::calculatePseudoLegalMoves({r, f}, tmpState)) {
                        danger.push_back(m.destination);
                    }
                }
            }
        }
        return danger;
    }

    static bool canCastle(const GameState& state, const bool kingSide) {
        // no check
        if(state.gameStatus == GameStatus::SINGLE_CHECK || state.gameStatus == GameStatus::DOUBLE_CHECK){
            return false;
        }
        // king hasn't moved
        const Coordinate kingStartPosition(state.currentPlayer == Color::WHITE ? 0 : 7, 4);
        Coordinate kingPosition = state.board.findKing(state.currentPlayer);
        const auto king = state.board.at(kingPosition);
        if(kingPosition != kingStartPosition || king->hasMoved()) {
            return false;
        }
        // rook hasn't moved
        const int rookFileOffset = kingSide ? 3 : -4;
        const Coordinate rookStartPosition = {kingPosition.rank, kingPosition.file + rookFileOffset};
        const auto rook = state.board.at(rookStartPosition);
        if (!rook || rook->type() != PieceType::ROOK || rook->hasMoved()) {
            return false;
        }
        // clear path
        if (!state.board.isPathClear(kingPosition, rookStartPosition)){
            return false;
        }

        // path squares cant be attacked
        const Color attackerColor = state.currentPlayer == Color::WHITE ? Color::BLACK : Color::WHITE;
        const int direction = kingSide ? 1 : -1;
        for (int step = 0; step <= 2; ++step) {
            const Coordinate square = {kingPosition.rank, kingPosition.file + direction * step};
            if (isSquareAttackedBy(square, attackerColor, state)) {
                return false;
            }
        }

        return true;
    }

    static bool isSquareAttackedBy(const Coordinate& target, const Color attackerColor, const GameState& gameState)
    {
        const GameState nextMoveState{attackerColor, gameState.gameStatus, gameState.board, gameState.lastMove};

        for (int r = 0; r < Board::BOARD_SIZE; ++r) {
            for (int f = 0; f < Board::BOARD_SIZE; ++f) {
                Coordinate source = {r, f};
                auto piece = gameState.board.at(source);
                if (!piece || piece->color() != attackerColor) continue;

                for (auto moves = calculatePseudoLegalMoves(source, nextMoveState); const auto& move : moves) {
                    if (move.destination == target && move.isCapture)
                        return true;
                }
            }
        }
        return false;
    }

    static std::vector<Move> calculateRegularMoves(const Board& board, const Coordinate& source) {
        const auto piece = board.at(source);
        if (!piece) return {};

        std::vector<Move> moves;

        for (const auto& dir : piece->getMoveDirections()) {
            int rank = source.rank;
            int file = source.file;

            do {
                rank += dir[0];
                file += dir[1];

                Coordinate destination {rank, file};

                if (!Board::inBounds(destination)) break;

                if (board.isFriendly(destination, piece->color())) break;

                Move move {source, destination, piece->color()};
                move.movingPieceType = piece->type();
                if (board.at(destination)) {
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

    static bool leavesKingInCheck(const Move& move, const GameState& state)
    {
        auto copy = state.board.clone();
        const auto movingPiece = copy.at(move.source);
        const Color movingColor = movingPiece->color();
        const Color enemy = (movingColor == Color::WHITE) ? Color::BLACK : Color::WHITE;

        // remove the en-passant captured pawn before checking for check
        if (move.moveType == MoveType::ENPASSANT) {
            const int capturedPawnRank = move.source.rank;
            const int capturedPawnFile = move.destination.file;
            copy.set({capturedPawnRank, capturedPawnFile}, nullptr);
        }

        // simulate the move
        copy.move(move.source, move.destination);

        const Coordinate kingSquare = copy.findKing(movingColor);
        const GameState tmpState = {state.currentPlayer, state.gameStatus, copy, move};

        return isSquareAttackedBy(kingSquare, enemy, tmpState);
    }
};