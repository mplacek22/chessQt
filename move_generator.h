#pragma once

#include "Move.h"
#include "game_state.h"
#include <unordered_map>
#include <vector>

/**
 * @brief Chess move generator - calculates all legal moves. Applies chess move rules.
 *
 */
namespace move_generator {

    /**
     * @brief Movement directions for each non-pawn piece type.
     *
     * Maps each piece type to its set of possible movement directions, where each
     * direction is a {rank_delta, file_delta} pair.
     *
     * @note Pawns are omitted as their movement is context-dependent (color, first move,
     * captures) and handled separately.
     */
    const std::unordered_map<PieceType, std::array<std::array<int, 2>, 28> > MOVE_DIRECTIONS = {
        {PieceType::BISHOP, {{{1, 1}, {1, -1}, {-1, 1}, {-1, -1}}}},
        {
            PieceType::KING, {
                {
                    {1, 0}, {-1, 0}, {0, 1}, {0, -1},
                    {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
                }
            }
        },
        {
            PieceType::KNIGHT, {
                {
                    {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
                    {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
                }
            }
        },
        {
            PieceType::QUEEN, {
                {
                    {1, 0}, {-1, 0}, {0, 1}, {0, -1},
                    {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
                }
            }
        },
        {PieceType::ROOK, {{{1, 0}, {-1, 0}, {0, 1}, {0, -1}}}}
    };

    /**
     * @brief Bitmask identifying sliding piece types.
     *
     * A piece is sliding if it can move any number of squares along a direction
     * until blocked.
     */
    constexpr int SLIDING_PIECE_MASK =
            (1 << static_cast<int>(PieceType::BISHOP)) |
            (1 << static_cast<int>(PieceType::QUEEN)) |
            (1 << static_cast<int>(PieceType::ROOK));

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
    std::vector<Move> calculatePossibleMoves(const Coordinate &source, const GameState &state);

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
    std::vector<Coordinate> computeCheckers(const GameState &gameState);


    /**
     * @brief Determines whether the current player can perform any legal move.
     *
     * It is used to detect checkmate and stalemate
     * If player is in check and this returns @c false it is checkmate.
     * If not in check and this returns @c false it is stalemate.
     *
     * @param gameState Current game state.
     * @return @c true if @p player has at least one legal move.
     * */
    bool canCurrentPlayerMove(const GameState &gameState);

    /**
     * @brief Determines whether @c state.currentPlayer can castle king side.
     *
     * @param state Current game state.
     * @return @c true if king side castling is currently legal.
     */
    bool canCastleKingSide(const GameState &state);

    /**
     * @brief Determines whether @c state.currentPlayer can castle queen side.
     *
     * @param state Current game state.
     * @return @c true if queen side castling is currently legal.
     */
    bool canCastleQueenSide(const GameState &state);
};
