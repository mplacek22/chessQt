#pragma once

#include "DrawCause.h"
#include "Move.h"
#include "game_state.h"
#include <vector>

/**
* @brief Interface decoupling game logic layer from presentation layer.
*
* The mediator interface declares a method used by components
* to notify the mediator about various events. The mediator may
* react to these events and pass the execution to other components.
*
* The communication is split into 2 directions:
* 1. Requests - UI -> Game
* 2. Notifications - Game -> UI
*/
class IGameMediator {
public:
    virtual ~IGameMediator() = default;

    // Requests---------------------------
    /**
     * @brief Requests the game to start.
    */
    virtual void onStartRequested() = 0;

    /**
    * @brief Requests execution of a player move.
    * @param move Move to be executed
    */
    virtual void onMoveRequested(Move &move) = 0;

    /**
    * @brief Requests promotion of a pawn to the specified piece type.
    * @param type Piece type the pawn should be promoted to.
    */
    virtual void onPromotionRequested(PieceType type) = 0;

    /**
    * @brief Requests the game to restart.
*/
    virtual void onRestartRequested() = 0;

    /**
     * @brief Requests the set of legal moves for the piece at the given coordinate.
     *
     * The result is delivered asynchronously via @c onPossibleMovesCalculated().
     *
     * @param coord Coordinate of the piece to query.
     */
    virtual void onPossibleMovesRequested(Coordinate coord) = 0;

    // Notifications---------------------------
    /**
     * @brief Notifies the UI that the game state has changed.
     *
     * Called after every successfully executed move.
     *
     * @param state Current game state after the move.
     */
    virtual void onGameStateChanged(const GameState &state) = 0;

    /**
     * @brief Notifies the UI that the game has been won.
     *
     * @param winner Color of the winning player.
     */
    virtual void onGameWon(Color winner) = 0;

    /**
     * @brief Notifies the UI that the game has ended in a draw.
     *
     * @param cause Reason for the draw (stalemate, insufficient material, etc.).
     */
    virtual void onGameDrawn(DrawCause cause) = 0;

    /**
      * @brief Notifies the UI whether a promotion decision is currently pending.
      *
      * @param isPromotionPending @c true if a pawn is awaiting promotion, @c false otherwise.
      */
    virtual void onPromotionPending(bool isPromotionPending) = 0;

    /**
        * @brief Delivers the legal moves calculated for the previously requested square.
        *
        * Called in response to @c onPossibleMovesRequested().
        *
        * @param moves List of legal moves available from the requested coordinate.
        */
    virtual void onPossibleMovesCalculated(const std::vector<Move> &moves) = 0;
};
