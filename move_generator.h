#pragma once

#include "Move.h"
#include "board.h"
#include "game_state.h"
#include <algorithm>

struct PinInfo {
    Coordinate pinnedPiecePosition;
    std::vector<Coordinate> allowedSquares; // squares the pinned piece can move to
};

class MoveGenerator
{
public:

    // All moves ignoring whether they leave the king in check
    static std::vector<std::shared_ptr<Move>> calculatePseudoLegalMoves(const Board& board, const Coordinate& source, const GameState& state)
    {
        auto piece = board.getPieceAt(source);
        if (!piece) return {};
        switch (piece->type()) {
            case PieceType::PAWN:   return calculatePawnMoves(board, source, state);
            default:                return calculateRegularMoves(board, source); // B, R, Q, N, K
        }
    }

    // Filters pseudo-legal moves to only those that are fully legal
    static std::vector<std::shared_ptr<Move>> calculatePossibleMoves(const Board& board, const Coordinate& source, const GameState& state)
    {
        auto piece = board.getPieceAt(source);
        if (!piece) return {};

        // in double check only the king can move
        if(state.gameStatus == GameStatus::DOUBLE_CHECK && board.getPieceAt(source)->type() != PieceType::KING) {
            return {};
        }

        std::vector<std::shared_ptr<Move>> pseudo;
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

        std::vector<std::shared_ptr<Move>> legal;
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

        for (int r = 0; r < board.board().size(); ++r) {
            for (int f = 0; f < board.board().size(); ++f) {
                Coordinate source {r, f};
                auto piece = board.getPieceAt(source);
                if (!piece || piece->color() != gameState.currentPlayer) continue;

                auto moves = calculatePseudoLegalMoves(board, source, gameState);
                for (const auto& move : moves) {
                    if (move->destination == kingPosition) {
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
        auto moves = calculateRegularMoves(board, source);
        auto dangerSquares = computeKingDangerSquares(board, source, state);

        // Filter out moves to king danger squares
        std::erase_if(moves, [&](const auto& m) {
            return std::ranges::find(dangerSquares, m->destination) != dangerSquares.end();
        });

        return moves;
    }

    static std::vector<Coordinate> computeKingDangerSquares(const Board& board,const Coordinate& kingPosition, const GameState& gameState)
    {
        Color color = board.getPieceAt(kingPosition)->color();
        std::array<std::array<std::shared_ptr<Piece>, 8>, 8> boardWithoutKing = board.board();
        boardWithoutKing[kingPosition.rank()][kingPosition.file()] = nullptr;

        std::vector<Coordinate> danger;
        for (int r = 0; r < boardWithoutKing.size(); r++) {
            for (int f = 0; f < boardWithoutKing.size(); f++) {
                const auto piece = boardWithoutKing[r][f];
                if (piece && piece->color() != color) {
                    for (const auto&  m : MoveGenerator::calculatePseudoLegalMoves(board, {r, f}, gameState)) {
                        danger.push_back(m->destination);
                    }
                }
            }
        }
        return danger;
    }

    // static bool canCastleKingSide(const Board& board, const GameState& state) {

    // }

    static bool isSquareAttackedBy(const Board& board, const Coordinate& target, Color attackerColor, const GameState& gameState)
    {
        for (int r = 0; r < board.board().size(); ++r) {
            for (int f = 0; f < board.board().size(); ++f) {
                Coordinate source = {r, f};
                auto piece = board.getPieceAt(source);
                if (!piece || piece->color() != attackerColor) continue;

                auto moves = calculatePseudoLegalMoves(board, source, gameState);

                for (const auto& move : moves) {
                    if (move->destination == target && move->capturedPiece)
                        return true;
                }
            }
        }
        return false;
    }

    static std::vector<std::shared_ptr<Move>> calculateRegularMoves(const Board& board, const Coordinate& source) {
        std::shared_ptr<Piece> piece = board.getPieceAt(source);
        if (!piece) return {};

        std::vector<std::shared_ptr<Move>> moves;

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

    static std::vector<PinInfo> computePins(const Board& board, const GameState& gameState)
    {
        std::vector<PinInfo> pins;
        Coordinate kingPos = board.findKing(gameState.currentPlayer);

        static const std::vector<std::pair<int,int>> allDirs = {
            {1,0},{-1,0},{0,1},{0,-1},   // straight
            {1,1},{1,-1},{-1,1},{-1,-1}  // diagonal
        };

        for (auto [df, dr] : allDirs) {
            // Ray from king outward, look for a friendly piece then an enemy slider
            Coordinate cursor{kingPos.rank() + dr, kingPos.file() + df};
            Coordinate candidatePinned{-1, -1};   // first friendly piece found on ray

            while (board.inBounds(cursor)) {
                auto piece = board.getPieceAt(cursor);

                if (!piece) {
                    cursor = {cursor.rank() + dr, cursor.file() + df};
                    continue;
                }

                if (piece->color() == gameState.currentPlayer) {
                    if (candidatePinned.rank() != -1) break; // second friendly -> no pin
                    candidatePinned = cursor; // first friendly -> candidate
                } else {
                    // Enemy piece -> check if it can slide along this ray
                    bool canPin = false;
                    if (piece->isSliding()) {
                        for (const auto& pieceDir : piece->getMoveDirections()) {
                            // Enemy piece attacks along this ray if one of its directions
                            // is the OPPOSITE of our outward ray from the king
                            if (pieceDir[0] == -dr && pieceDir[1] == -df) {
                                canPin = true;
                                break;
                            }
                        }
                    }

                    if (canPin && candidatePinned.rank() != -1) {
                        // ── Step 3: overlap confirmed — candidatePinned is pinned ──
                        // Legal squares = ray from king up to and including the pinner
                        PinInfo pin {candidatePinned, {}};

                        // Collect allowed squares: king → pinned → pinner (inclusive)
                        Coordinate sq{kingPos.rank() + dr, kingPos.file() + df};
                        while (board.inBounds(sq)) {
                            pin.allowedSquares.push_back(sq);
                            if (board.getPieceAt(sq)) break; // stop at pinner (inclusive)
                            sq = {sq.rank() + dr, sq.file() + df};
                        }

                        pins.push_back(pin);
                    }
                    break; // enemy piece ends the ray regardless
                }
                cursor = {cursor.rank() + dr, cursor.file() + df};
            }
        }

        return pins;
    }

    static std::vector<std::shared_ptr<Move>> filterPinnedMoves(const Board& board,
                                                                const Coordinate& source,
                                                                const std::vector<std::shared_ptr<Move>>& moves,
                                                                const GameState& gameState)
    {
        if (moves.empty()) return moves;

        auto pins = computePins(board, gameState);

        auto it = std::find_if(pins.begin(), pins.end(),
                               [&source](const PinInfo& pin) {
                                   return pin.pinnedPiecePosition == source;
                               });

        // not pinned
        if (it == pins.end()) return moves;

        // knights cannot move when pinned
        if (board.getPieceAt(source)->type() == PieceType::KNIGHT) return {};

        const auto& allowedSquares = it->allowedSquares;
        std::vector<std::shared_ptr<Move>> filtered;
        filtered.reserve(moves.size());
        std::copy_if(moves.begin(), moves.end(), std::back_inserter(filtered),
                     [&allowedSquares](const std::shared_ptr<Move>& move) {
                         return std::find(allowedSquares.begin(), allowedSquares.end(),
                                          move->destination) != allowedSquares.end();
                     });

        return filtered;
    }

    static bool leavesKingInCheck(const std::shared_ptr<Move> move, const Board& board, const GameState& state)
    {
        Board copy = board;
        auto movingPiece = copy.getPieceAt(move->source);
        copy.movePiece(move->source, move->destination);

        //todo: handle en passant
        auto kingSquare = copy.findKing(movingPiece->color());
        Color enemy = (movingPiece->color() == Color::WHITE) ? Color::BLACK : Color::WHITE;
        return isSquareAttackedBy(copy, kingSquare, enemy, state);
    }
};