#ifndef GAME_H
#define GAME_H

#include "gameStatus.h"
#include "Board.h"
#include "Move.h"
#include "game_state.h"

#include <vector>
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

    [[nodiscard]] const std::vector<Move>& movesHistory() const { return movesHistory_; }

    void processMove(Move &currentMove);

    void calculatePossibleMovesForPiece(Coordinate& source);

    bool pendingPromotion() const { return pendingPromotion_; }

    std::optional<Coordinate> promotionSquare() const { return promotionSquare_; }

    void promotePawn(PieceType type);

    const GameState gameState() const;

private:
    Color currentPlayer_ = Color::WHITE;
    Board board_ = Board();
    std::vector<Move> movesHistory_;
    GameStatus status_ = GameStatus::NEW;
    bool pendingPromotion_ = false;
    std::optional<Coordinate> promotionSquare_;

    void switchPlayer();

    void executeMove(const Move &move);

    void updateGameStatus();
};

#endif // GAME_H
