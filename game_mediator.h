#pragma once

#include "Game.h"
#include "game_client.h"
#include "i_game_mediator.h"

/**
* @class GameMediator
* @brief Concrete mediator that wires a @c Game instance to a @c GameClient.
*/
class GameMediator final : public IGameMediator {
public:
    /**
     * @brief Constructs the mediator and registers it with both collaborators.
     *
     * @param game Non-owning pointer to the game logic instance.
     * @param gameClient Non-owning pointer to the presentation layer instance.
     */
    GameMediator(Game *game, IGameClient *gameClient);

    // Requests
    /// @copydoc IGameMediator::onPromotionRequested
    void onPromotionRequested(PieceType type) override;

    /// @copydoc IGameMediator::onRestartRequested
    void onRestartRequested() override;

    /// @copydoc IGameMediator::onStartRequested
    void onStartRequested() override;

    /// @copydoc IGameMediator::onMoveRequested
    void onMoveRequested(Move &move) override;

    /// @copydoc IGameMediator::onPossibleMovesRequested
    void onPossibleMovesRequested(Coordinate coord) override;

    // Notifications
    /// @copydoc IGameMediator::onGameStateChanged
    void onGameStateChanged(const GameState &state) override;

    /// @copydoc IGameMediator::onPossibleMovesCalculated
    void onPossibleMovesCalculated(const std::vector<Move> &moves) override;

    /// @copydoc IGameMediator::onGameWon
    void onGameWon(Color winner) override;

    /// @copydoc IGameMediator::onGameDrawn
    void onGameDrawn(DrawCause cause) override;

    /// @copydoc IGameMediator::onPromotionPending
    void onPromotionPending(bool isPromotionPending) override;

private:
    Game *game_;
    IGameClient *gameClient_;
};
