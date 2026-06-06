#pragma once

#include "gameStatus.h"
#include "Board.h"
#include "Move.h"
#include "game_state.h"
#include "colleague.h"

#include <DrawCause.h>
#include <vector>

class Game : public Colleague
{
public:
    Game();
    void start();

    void restart();

    [[nodiscard]] const Board& board() const { return board_; }

    [[nodiscard]] Color currentPlayer() const { return currentPlayer_; }

    [[nodiscard]] GameStatus status()   const { return status_; }

    [[nodiscard]] const std::vector<Move>& movesHistory() const { return movesHistory_; }

    void processMove(Move &currentMove);

    [[nodiscard]] bool pendingPromotion() const { return pendingPromotionMove_.has_value(); }

    void promotePawn(PieceType type);

    [[nodiscard]] GameState gameState() const;

    [[nodiscard]] std::optional<DrawCause> drawCause() const { return drawCause_; }

    [[nodiscard]] std::optional<Color> winner() const { return winner_; }

    [[nodiscard]] bool isGameOngoing() const;

    void requestPossibleMoves(const Coordinate& coord);


private:
    Color currentPlayer_ = Color::WHITE;
    Board board_ = Board();
    std::vector<Move> movesHistory_;
    GameStatus status_ = GameStatus::NEW;
    std::optional<Move> pendingPromotionMove_;
    std::optional<Color> winner_;
    std::optional<DrawCause> drawCause_;
    static constexpr int ONGOING_GAME_STATUSES_MASK =
        (1 << static_cast<int>(GameStatus::IN_PROGRESS)) |
        (1 << static_cast<int>(GameStatus::SINGLE_CHECK)) |
        (1 << static_cast<int>(GameStatus::DOUBLE_CHECK));

    void switchPlayer();

    void executeMove(const Move &move);

    void updateGameStatus();

    void updateWinner();

    [[nodiscard]] bool isFiftyMoveRule() const;

    [[nodiscard]] bool isRepetition() const;

    [[nodiscard]] bool isInsufficientMaterial() const;

    [[nodiscard]] static Color oppositeColor(Color color);

    void setDraw(DrawCause drawCause);

    void setPendingPromotionMove(std::optional<Move> moveOpt);
};