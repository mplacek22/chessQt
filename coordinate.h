#pragma once

/**
 * @struct Coordinate
 * @brief Represents chess square coordinate.
 *
 */
struct Coordinate {
    /**
     * @brief Represents row of the board.
     */
    int rank;

    /**
     * @brief Represents column of the board.
     */
    int file;

    bool operator==(const Coordinate &other) const {
        return rank == other.rank && file == other.file;
    }
};
