#pragma once

#include "Move.h"

#include <string>


/**
 * @brief Parses chess moves to and from Standard Algebraic Notation (SAN).
 */
namespace san {
    /**
     * @brief Parses @c Move to string SAN.
     *
     * @param move The move to parse to SAN.
     * @return A string representing the move in SAN.
     */
    std::string toSAN(const Move &move);

    /**
     * @brief Parses raw string to @c Coordinate.
     *
     * @p raw must be exactly 2 chars long.
     * First char is file.
     * File must be between 'a' and 'h'.
     * Second char is rank.
     * Rank must be between '1' and '8'.
     *
     * @param raw Raw string to be parsed.
     * @return A coordinate parsed from string.
     * @throws InvalidSanException when coordinate is invalid.
     */
    Coordinate coordinateFromString(const std::string &raw);

    /**
     * @brief Parses char to corresponding PieceType
     * @param c char to be parsed
     * @return A PieceType parsed from char
     */
    PieceType charToPieceType(char c);
};
