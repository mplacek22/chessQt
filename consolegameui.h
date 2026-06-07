#pragma once

#include "game_client.h"


/**
 * @brief Console based chess UI.
 *
 * It drives the game loop, renders the board, and handles player
 * input including move entry, pawn promotion, and game control commands.
 *
 * Supported commands (entered at any coordinate prompt):
 * - @c q / @c quit - exit the application.
 * - @c r / @c restart - restart the game to the initial setup.
 */
class ConsoleGameUI final : public IGameClient {
public:
    /**
     * @brief Runs the main game loop until the game ends or the player quits.
     *
     * 1. Displays the current player.
     * 2. Displays the current game status.
     * 3. Displays the board.
     * 4. Prompts the user for move source square.
     * 5. Prompts the user for move destination square.
     * 6. If move is invalid displays the information and prompts again.
     * 7. Requests a move from mediator.
     * 8. Returns when the game reaches a terminal state (checkmate,
     */
    void run();

    /**
     * @brief possibleMovesCalculated Receives legal moves from the game mediator.
     * @param moves Legal moves for the previously selected source square.
     */
    void possibleMovesCalculated(std::vector<Move> moves) override;

    /**
    * @brief onGameStateChanged Receives a full game state update from mediator and
    * refreshes all bound properties.
    *
    * @param gameState Current game state after the latest move.
    */
    void onGameStateChanged(const GameState &gameState) override;

    /**
     * @brief onGameWon Receives a win notification,
     *  updates the status properties and displays the info.
     * @param winner Color of the winning player.
     */
    void onGameWon(Color winner) override;

    /**
     * @brief onGameDrawn Receives a draw notification,
     * updates the status properties and  displays the info.
     * @param drawCause Cause of the draw.
     */
    void onGameDrawn(DrawCause drawCause) override;

    /**
     * @brief Notifies the UI of a pending promotion and prompts for a choice.
     *
     * Blocks until the player enters a valid piece character
     * (@c Q, @c R, @c B, or @c N), then forwards the choice to the mediator.
     *
     * @param pendingPromotion @c true when a promotion choice is required.
     */
    void onPendingPromotionChanged(bool pendingPromotion) override;

private:
    enum class PromptResult {
        COORDINATE,
        QUIT,
        RESTART,
        INVALID
    };

    enum class LoopAction {
        QUIT,
        RESTART,
    };

    struct PromptOutcome {
        PromptResult result;
        std::optional<Coordinate> coordinate;
    };

    void displayBoard() const;

    void displayCurrentPlayer() const;

    void displayStatus() const;

    static void displayEndGameOptions();

    [[nodiscard]] static char pieceChar(PieceType type, Color color);

    [[nodiscard]] static PromptOutcome promptCoordinate(const std::string &prompt);

    [[nodiscard]] static std::string drawCauseToString(DrawCause drawCause);

    [[nodiscard]] static std::string gameStatusToString(GameStatus gameStatus);

    [[nodiscard]] LoopAction runGameLoop();

    [[nodiscard]] LoopAction promptEndGame() const;

    static constexpr std::array<PieceType, 4> PROMOTION_PIECES = {
        PieceType::QUEEN, PieceType::KNIGHT, PieceType::BISHOP, PieceType::ROOK
    };
};
