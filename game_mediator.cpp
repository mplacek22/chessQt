#include "game_mediator.h"

GameMediator::GameMediator(Game *game, IGameClient *gameClient)
    : game_(game), gameClient_(gameClient) {
    game_->setMediator(this);
    gameClient_->setMediator(this);
}

void GameMediator::onPromotionRequested(PieceType type) {
    game_->promotePawn(type);
}

void GameMediator::onRestartRequested() {
    game_->restart();
}

void GameMediator::onStartRequested()
{
    game_->start();
}

void GameMediator::onPossibleMovesRequested(Coordinate coord)
{
    game_->requestPossibleMoves(coord);
}

void GameMediator::onMoveRequested(Move &move)
{
    game_->processMove(move);
}

void GameMediator::onGameStateChanged(const GameState &state)
{
    gameClient_->onGameStateChanged(state);
}

void GameMediator::onGameWon(Color winner)
{
    gameClient_->onGameWon(winner);
}

void GameMediator::onGameDrawn(DrawCause cause)
{
    gameClient_->onGameDrawn(cause);
}

void GameMediator::onPromotionPending(bool isPromotionPending)
{
    gameClient_->onPendingPromotionChanged(isPromotionPending);
}



void GameMediator::onPossibleMovesCalculated(const std::vector<Move> &moves)
{
    gameClient_->possibleMovesCalculated(moves);
}
