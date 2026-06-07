#pragma once

#include "Color.h"
#include "DrawCause.h"
#include "Move.h"
#include "colleague.h"

#include <vector>

/**
 * @brief Base interface for game clients.
 *
 * The class receives notifications from the game mediator and caches the latest game state.
 * It can also call the game via mediator.
 */
class IGameClient : public Colleague {
public:
    IGameClient() = default;

    /**
     * @brief Updates cached game winner
     *
     * @param winner Color of the winning player.
     */
    virtual void onGameWon(Color winner);

    /**
    * @brief Updates cached draw cause.
    *
    * @param drawCause Reason for the draw.
    */
    virtual void onGameDrawn(DrawCause drawCause);

    /**
     * @brief Updates cached pawn pending promotion state.
     *
     * @param pendingPromotion @c true if a pawn is waiting for promotion, @c false otherwise.
     */
    virtual void onPendingPromotionChanged(bool pendingPromotion);

    /**
     * @brief Updates the cached board, status and last move when game state changed.
     *
     * @param gameState Current game state.
     */
    virtual void onGameStateChanged(const GameState &gameState);

    /**
    * @brief Updates the cached possible moves
    *
    * @param moves List of legal moves.
    */
    virtual void possibleMovesCalculated(std::vector<Move> moves);

    /**
    * @brief Requests pawn promotion to specified pieceType.
    */
    void promotePawn(PieceType pieceType);

    /**
    * @brief Requests game start.
   */
    void startGame();

    /**
    * @brief Requests game restart
   */
    void restartGame();

protected:
    /// Cached information whether a pawn promotion is pending.
    bool cachedPendingPromotion_ = false;

    /// Cached player whose turn it is.
    Color cachedCurrentPlayer_ = Color::WHITE;

    /// Cached game status.
    GameStatus cachedGameStatus_ = GameStatus::NEW;

    /// Cached list of currently possible moves.
    std::vector<Move> cachedPossibleMoves_;

    /// Cached winner, if the game has ended with a victory.
    std::optional<Color> winner_;

    /// Cached draw cause, if the game ended in a draw.
    std::optional<DrawCause> drawCause_;

    /// Cached board state.
    Board cachedBoard_;

    /**
     * @brief Checks whether the game is currently ongoing.
     *
     * @return True if the game is not finished, false otherwise.
     */
    [[nodiscard]] bool isGameOngoing() const;

private:
    /// Bit mask of game statuses considered to be ongoing.
    static constexpr int ONGOING_GAME_STATUSES_MASK =
            1 << static_cast<int>(GameStatus::IN_PROGRESS) |
            1 << static_cast<int>(GameStatus::SINGLE_CHECK) |
            1 << static_cast<int>(GameStatus::DOUBLE_CHECK);
};
