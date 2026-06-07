#pragma once

#include "Board.h"
#include "Color.h"
#include "Move.h"
#include "gameStatus.h"

/**
 * @struct GameState
 * @brief Snapshot of a game state
 */
struct GameState {

    /// Color of the player whose turn it is to move.
    Color currentPlayer;

    /// Current status of the game
    GameStatus gameStatus;

    /// The last move played or @c std::nullopt at the start of the game.
    std::optional<Move> lastMove;

    /// Current board state reference
    const Board& board;

    GameState(const Color player, const GameStatus gameStatus, const Board& board, const std::optional<Move> &lastMove = std::nullopt)
        :currentPlayer(player), gameStatus(gameStatus), lastMove(lastMove), board(board) {}
};
