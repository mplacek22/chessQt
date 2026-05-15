#pragma once

#include "Coordinate.h"

#include <array>


template<typename T, int Rows = 8, int Cols = 8>
class Grid
{
public:
    Grid() = default;
    const T& at(int row, int col) const
    {
        if (!inBounds(row, col))
            throw std::out_of_range("Grid::at — coordinate out of bounds");

        return data[row][col];
    }
    const T& at(const Coordinate& coordinate) const
    {
        return at(coordinate.rank(), coordinate.file());
    }
    void set(int row, int col, T val)
    {
        if (!inBounds(row, col))
            throw std::out_of_range("Grid::set — coordinate out of bounds");

        data[row][col] = std::move(val);
    }
    void set(const Coordinate& coordinate, T val)
    {
        set(coordinate.rank(), coordinate.file(), val);
    }
    void move(int rowSrc, int colSrc, int rowDest, int colDest)
    {
        if (!inBounds(rowSrc, colSrc) || !inBounds(rowDest, colDest))
            throw std::out_of_range("Grid::move — coordinate out of bounds");

        data[rowDest][colDest] = std::move(data[rowSrc][colSrc]);
        data[rowSrc][colSrc] = T{};
    }
    void move(const Coordinate& source, const Coordinate& destination)
    {
        move(source.rank(), source.file(), destination.rank(), destination.file());
    }
    bool inBounds(int row, int col) const
    {
        return row >= 0 && row < Rows && col >= 0 && col < Cols;
    }
    bool inBounds(const Coordinate& coordinate) const
    {
        return inBounds(coordinate.rank(), coordinate.file());
    }
protected:
    std::array<std::array<T, Cols>, Rows> data;
};