#ifndef GAME_H
#define GAME_H

#include "gameStatus.h"
#include "Board.h"
#include "Move.h"
#include <stack>
// #include <unordered_set>

class Game
{
public:
    Game();
    void start();

    void restart();

    void exit();

    [[nodiscard]] Board& board() { return board_; }
    [[nodiscard]] Color currentPlayer() const { return currentPlayer_; }
    [[nodiscard]] GameStatus status()   const { return status_; }

    void processMove(Move &currentMove);

private:
    Color currentPlayer_ = Color::WHITE;
    Board board_ = Board();
    std::stack<Move> movesHistory_;
    GameStatus status_ = GameStatus::NEW;

    void switchPlayer();

    void executeMove(const Move &move);

    void updateGameStatus();
};

#endif // GAME_H
