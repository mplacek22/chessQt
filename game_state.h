#pragma once

#include "Color.h"
#include "Move.h"
#include "gameStatus.h"

struct GameState {
    Color currentPlayer;
    GameStatus gameStatus;
    std::optional<Move> lastMove;
};
