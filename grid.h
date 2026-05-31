#pragma once

#include <array>
#include <stdexcept>


template<typename T, std::size_t Rows  = 8, std::size_t Cols = 8>
class Grid
{
public:
    static constexpr std::size_t SIZE = Rows * Cols;

    Grid() = default;
    Grid(Grid&&) = default;
    Grid& operator=(Grid&&) = default;

    T& at(const int row, const int col)
    {
        return data[index(row, col)];
    }

    const T& at(const int row, const int col) const
    {
        if (!inBounds(row, col))
            throw std::out_of_range("Grid::at — coordinate out of bounds");

        return data[index(row, col)];
    }

    void set(const int row, const int col, T val)
    {
        if (!inBounds(row, col))
            throw std::out_of_range("Grid::set — coordinate out of bounds");

        data[index(row, col)] = std::move(val);
    }

    void move(const int rowSrc, const int colSrc, const int rowDest, const int colDest)
    {
        if (!inBounds(rowSrc, colSrc) || !inBounds(rowDest, colDest))
            throw std::out_of_range("Grid::move — coordinate out of bounds");

        data[index(rowDest, colDest)] = std::move(data[index(rowSrc, colSrc)]);
        data[index(rowSrc, colSrc)] = T{};
    }

    [[nodiscard]] static bool inBounds(int row, int col) {
        return row >= 0 && row < Rows && col >= 0 && col < Cols;
    }

    [[nodiscard]] static std::size_t size() noexcept { return SIZE; }

    void clear() {
        data = {};
    }

protected:
    std::array<T, Rows * Cols> data;

    [[nodiscard]] static std::size_t index(const int row, const int col) {
        if (!inBounds(row, col))
            throw std::out_of_range("Grid::index — coordinate out of bounds");
        return static_cast<std::size_t>(row) * Cols
               + static_cast<std::size_t>(col);
    }
};