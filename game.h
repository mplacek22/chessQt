#ifndef GAME_H
#define GAME_H

#include "gameStatus.h"
#include "Board.h"
#include "Move.h"
#include "game_state.h"
#include "IGameObserver.h"

#include <DrawCause.h>
#include <vector>
// #include <unordered_set>

class Game
{
public:
    Game();
    void start();

    void restart();

    void exit();

    [[nodiscard]] const Board& board() const { return board_; }

    [[nodiscard]] Color currentPlayer() const { return currentPlayer_; }

    [[nodiscard]] GameStatus status()   const { return status_; }

    [[nodiscard]] const std::vector<Move>& movesHistory() const { return movesHistory_; }

    void processMove(Move &currentMove);

    bool pendingPromotion() const { return pendingPromotion_; }

    std::optional<Coordinate> promotionSquare() const { return promotionSquare_; }

    void promotePawn(PieceType type);

    const GameState gameState() const;

    const std::optional<DrawCause> drawCause() const { return drawCause_; }

    const std::optional<Color> winner() { return winner_; }

    void setObserver(std::weak_ptr<IGameObserver> observer) {
        observer_ = std::move(observer);
    }

    bool isGameOngoing() const;

private:
    Color currentPlayer_ = Color::WHITE;
    Board board_ = Board();
    std::vector<Move> movesHistory_;
    GameStatus status_ = GameStatus::NEW;
    bool pendingPromotion_ = false;
    std::optional<Coordinate> promotionSquare_;
    std::optional<Color> winner_;
    std::optional<DrawCause> drawCause_;
    std::weak_ptr<IGameObserver> observer_;
    static constexpr int ONGOING_GAME_STATUSES_MASK =
        (1 << static_cast<int>(GameStatus::IN_PROGRESS)) |
        (1 << static_cast<int>(GameStatus::SINGLE_CHECK)) |
        (1 << static_cast<int>(GameStatus::DOUBLE_CHECK));

    void switchPlayer();

    void executeMove(const Move &move);

    void updateGameStatus();

    void updateWinner();

    bool isFiftyMoveRule() const;

    bool isRepetition() const;

    bool isInsufficientMaterial() const;

    Color oppositeColor(Color color) const;

    void setDraw(DrawCause drawCause);

};

#endif // GAME_H
