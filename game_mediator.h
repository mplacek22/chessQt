#ifndef GAME_MEDIATOR_H
#define GAME_MEDIATOR_H

#include "Game.h"
#include "game_client.h"
#include "i_game_mediator.h"


class GameMediator : public IGameMediator {
public:
    GameMediator(Game* game, GameClient* gameClient)
        : game_(game), gameClient_(gameClient) {
        game_->setMediator(this);
        gameClient_->setMediator(this);
    }

    // UI -> game
    void onPromotionRequested(PieceType type) override;
    void onRestartRequested() override;
    void onStartRequested() override;
    void onMoveRequested(Move &move) override;

    // game -> UI
    void onGameStateChanged(const GameState &state) override;
    void onPossibleMovesCalculated(const std::vector<Move> &moves) override;
    void onGameWon(Color winner) override;
    void onGameDrawn(DrawCause cause) override;
    void onPromotionPending(bool isPromotionPending) override;
    void onPossibleMovesRequested(Coordinate coord) override;

private:
    Game* game_;
    GameClient* gameClient_;
};

#endif // GAME_MEDIATOR_H
