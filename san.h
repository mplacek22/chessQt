#pragma once

#include "Board.h"
#include "game_state.h"

/**
 * @brief Converts chess moves to and from Standard Algebraic Notation (SAN)
 */
namespace san
{
 /**
  * @brief Converts {@link Move} to string SAN
  *
  * @param move The move to convert to SAN
  * @return A string representing the move in SAN
  */
 std::string toSAN(const Move& move);

 /**
  * @brief Converts string SAN to {@link Move}
  *
  * @param san SAN string to parse
  * @param board the current board state
  * @param gameState the current game state
  * @return
  */
 Move fromSAN (const std::string& san, const Board& board, const GameState& gameState);
};

