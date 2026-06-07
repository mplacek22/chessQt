#pragma once

#include "gameStatus.h"
#include "Board.h"
#include "Move.h"
#include "game_state.h"
#include "colleague.h"
#include "DrawCause.h"

#include <vector>

/**
 * @class Game
 * @brief Core chess game logic.
 */
class Game final : public Colleague {
public:
    Game();

    /**
     * @brief Starts the game.
     *
     * Notifies the mediator.
     */
    void start();

    /**
     * @brief Restarts the game.
     *
     * Clears the game state:
     * - current player
     * - winner
     * - draw cause
     * - movesHistory_
     *
     * Restarts the board.
     *
     * Notifies the mediator.
     */
    void restart();

    /**
     * @brief Performs a move and updates the game.
     *
     * 1. Executes the move.
     * 2. If the move is a promotion move and the promotion
     * piece type is not specified the execution is paused.
     * 3. Appends the move to the move history.
     * 4. Updates the game status.
     * 5. Updates the last moves game status after move in the history.
     * 5. Switches the players turn.
     * 6. Notifies the mediator.
     *
     * @param currentMove Move to execute.
     */
    void processMove(const Move &currentMove);

    /**
     * @brief Resolves a pending pawn promotion.
     *
     * Replaces the promoting pawn with a piece of the given type.
     * Completes the move processing that was paused by @c processMove().
     *
     * @param type Piece type to promote to. Must not be @c PAWN or @c KING.
     */
    void promotePawn(PieceType type);

    /**
     * @brief Requests the set of legal moves for the piece at the given coordinate.
     *
     * Delegates to the move generator and delivers the result back to the
     * mediator via @c IGameMediator::onPossibleMovesCalculated().
     *
     * @param coord Coordinate of the piece to query.
     */
    void requestPossibleMoves(const Coordinate &coord) const;

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

    [[nodiscard]] bool pendingPromotion() const;

    [[nodiscard]] GameState gameState() const;

    void setDraw(DrawCause drawCause);

    void setPendingPromotionMove(const std::optional<Move> &moveOpt);
};
