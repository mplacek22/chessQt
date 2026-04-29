#pragma once
#include <stdexcept>

// template<size_t boardSize = 8>
struct Coordinate {
    Coordinate(const int rank, const int file) {
        this->file_ = file;
        this->rank_ = rank;
    }

    Coordinate(const std::string &chessNotation) {
        if (!validate(chessNotation)) {
            throw std::invalid_argument(
                "Invalid chess notation. File must be between 'a' and 'h' and rank between '1' and '8'");
        }
        file_ = chessNotation[0] - 'a';
        rank_ = chessNotation[1] - '1';
    }

    [[nodiscard]] std::string toChessNotation() const {
        return std::string{fileChessNotation(), rankChessNotation()};
    }

    bool operator==(const Coordinate &aCoordinate) const {
        return this->file() == aCoordinate.file() && this->rank() == aCoordinate.rank();
    };

    static bool validate(const std::string &chessNotation) {
        return chessNotation[0] >= 'a' && chessNotation[0] <= maxFile_ && chessNotation[1] >= '1' &&
               chessNotation[1] <= maxRank_;
    }

    [[nodiscard]] int file() const {
        return file_;
    }

    [[nodiscard]] int rank() const {
        return rank_;
    }

    [[nodiscard]] char fileChessNotation() const {
        return static_cast<char>('a' + file_);
    }

    [[nodiscard]] char rankChessNotation() const {
        return static_cast<char>('1' + rank_);
    }

private:
    int file_; //a-h
    int rank_; //1-8
    static const int BOARD_SIZE = 8;

    static constexpr char maxFile_ = 'a' + BOARD_SIZE - 1;
    static constexpr char maxRank_ = '1' + BOARD_SIZE - 1;

    static bool validate(const int file, const int rank) {
        return file >= 0 && rank >= 0 && file < BOARD_SIZE && rank < BOARD_SIZE;
    }
};
