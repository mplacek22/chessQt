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

    [[nodiscard]] const Board& board() const;

    [[nodiscard]] Color currentPlayer() const;

    [[nodiscard]] GameStatus status()   const;

    [[nodiscard]] const std::vector<Move>& movesHistory() const;

    void processMove(Move &currentMove);

    [[nodiscard]] bool pendingPromotion() const;

    void promotePawn(PieceType type);

    [[nodiscard]] GameState gameState() const;

    [[nodiscard]] std::optional<DrawCause> drawCause() const;

    [[nodiscard]] std::optional<Color> winner() const;

    void requestPossibleMoves(const Coordinate& coord);

private:
    Color currentPlayer_ = Color::WHITE;
    Board board_ = Board();
    std::vector<Move> movesHistory_;
    GameStatus status_ = GameStatus::NEW;
    std::optional<Move> pendingPromotionMove_;
    std::optional<Color> winner_;
    std::optional<DrawCause> drawCause_;

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