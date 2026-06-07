#include "game_client.h"


void IGameClient::possibleMovesCalculated(std::vector<Move> moves)
{
    cachedPossibleMoves_ = moves;
}

void IGameClient::onGameStateChanged(const GameState &gameState)
{
    cachedCurrentPlayer_ = gameState.currentPlayer;
    cachedGameStatus_ = gameState.gameStatus;
    cachedBoard_ = gameState.board;
}

void IGameClient::onGameWon(Color winner)
{
    winner_ = winner;
}

void IGameClient::onGameDrawn(DrawCause drawCause)
{
    drawCause_ = drawCause;
}

void IGameClient::onPendingPromotionChanged(bool pendingPromotion)
{
    cachedPendingPromotion_ = pendingPromotion;
}

void IGameClient::promotePawn(PieceType pieceType)
{
    mediator_->onPromotionRequested(static_cast<PieceType>(pieceType));
}

void IGameClient::startGame()
{
    mediator_->onStartRequested();
}

void IGameClient::restartGame()
{
    mediator_->onRestartRequested();
}

bool IGameClient::isGameOngoing() const
{
    return ONGOING_GAME_STATUSES_MASK & (1 << static_cast<int>(cachedGameStatus_));
}