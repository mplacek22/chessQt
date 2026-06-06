#pragma once

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
  * @param gameState the current game state
  * @return
  */
 Move fromSAN (const std::string& san, const GameState& gameState);

 Coordinate coordinateFromString(const std::string &raw);

 PieceType charToPieceType(char c);
};

