#pragma once

#include "Board.h"
#include "Color.h"
#include "Move.h"
#include "gameStatus.h"

struct GameState {
    Color currentPlayer;
    GameStatus gameStatus;
    std::optional<Move> lastMove;
    const Board& board;

    GameState(const Color player, const GameStatus gameStatus, const Board& board, const std::optional<Move> &lastMove = std::nullopt)
        :currentPlayer(player), gameStatus(gameStatus), lastMove(lastMove), board(board) {}
};
