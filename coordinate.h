#pragma once

struct Coordinate {
    int rank;
    int file;

    bool operator==(const Coordinate& other) const {
        return rank == other.rank && file == other.file;
    }
};
