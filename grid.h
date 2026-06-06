#pragma once

#include <array>
#include <stdexcept>

/**
 * @brief Generic 2D grid with fixed size.
 *
 * @tparam T Type of the elements stored in the grid
 * @tparam Rows Number of rows
 * @tparam Cols Number of columns
 *
 */
template<typename T, std::size_t Rows  = 8, std::size_t Cols = 8>
class Grid
{
public:
    /**
     * @brief Total number of cells.
     */
    static constexpr std::size_t SIZE = Rows * Cols;

    Grid() = default;
    Grid(const Grid&) = default;
    Grid& operator=(const Grid&) = default;
    Grid(Grid&&) = default;
    Grid& operator=(Grid&&) = default;

    /**
     * @brief Returns a const reference to specified cell.
     * @param row Row of the cell
     * @param col Column of the cell
     * @return Const reference to the element at (rank, file).
     * @throws std::out_of_range if coordinates are out of bounds.
     */
    const T& at(const int row, const int col) const
    {
        if (!inBounds(row, col))
            throw std::out_of_range("Grid::at — coordinate out of bounds");

        return data[index(row, col)];
    }

    /**
     * @brief Returns a mutable reference to specified cell.
     * @param row Row of the cell
     * @param col Column of the cell
     * @return Const reference to the element at (rank, file).
     * @throws std::out_of_range if coordinates are out of bounds.
     */
    T& at(const int row, const int col)
    {
        if (!inBounds(row, col))
            throw std::out_of_range("Grid::at — coordinate out of bounds");

        return data[index(row, col)];
    }

    /**
     * @brief Sets the value of the cell at the given coordinates.
     * @param row Row of the cell
     * @param col Column of the cell
     * @param val New value
     * @throws std::out_of_range if coordinates are out of bounds.
     */
    void set(const int row, const int col, T val)
    {
        if (!inBounds(row, col))
            throw std::out_of_range("Grid::set — coordinate out of bounds");

        data[index(row, col)] = std::move(val);
    }

    /**
     * @brief Moves an element from a source cell to a destination cell.
     *
     * The source cell is reset to default value of the T.
     *
     * @param rowSrc  Row of the source cell.
     * @param colSrc  Column of the source cell.
     * @param rowDest Row of the destination cell.
     * @param colDest Column of the destination cell.
     * @throws std::out_of_range if coordinates are out of bounds.
     */
    void move(const int rowSrc, const int colSrc, const int rowDest, const int colDest)
    {
        if (!inBounds(rowSrc, colSrc) || !inBounds(rowDest, colDest))
            throw std::out_of_range("Grid::move — coordinate out of bounds");

        auto& src = data[index(rowSrc, colSrc)];

        if (!src.has_value())
            throw std::logic_error("Grid::move — source cell is empty");

        data[index(rowDest, colDest)] = std::move(src);
        src.reset();
    }

    /**
     * @brief Checks whether the given coordinates are within the grid boundaries.
     *
     * @param row Row of the cell.
     * @param col Column of the cell.
     * @return @c true if (row, col) is a valid cell, @c false otherwise.
     */
    [[nodiscard]] static bool inBounds(const int row, const int col) {
        return row >= 0 && row < Rows && col >= 0 && col < Cols;
    }

    /**
     * @brief Returns the total number of cells in the grid.
     *
     * @return The value of @c SIZE
     */
    [[nodiscard]] static std::size_t size() noexcept { return SIZE; }

    /**
     * @brief Resets all cells to the default value.
     */
    void clear() {
        for (auto& cell : data) cell = T{};
    }

protected:
    /** @brief Contiguous row-major buffer storing all grid elements. */
    std::array<T, Rows * Cols> data;

    /**
     * @brief Calculates the flat buffer index for the given coordinates.
     *
     * @param row Row index.
     * @param col Column index.
     * @return The index into @c data corresponding to (row, col).
     * @throws std::out_of_range if coordinates are out of bounds.
     */
    [[nodiscard]] static std::size_t index(const int row, const int col) {
        if (!inBounds(row, col))
            throw std::out_of_range("Grid::index — coordinate out of bounds");
        return static_cast<std::size_t>(row) * Cols + static_cast<std::size_t>(col);
    }
};