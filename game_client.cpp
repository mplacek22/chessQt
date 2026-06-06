#include "game_client.h"


void GameClient::possibleMovesCalculated(std::vector<Move> moves)
{
    cachedPossibleMoves_ = moves;
}

static Coordinate indexToCoordinate(int index)
{
    int file = index % 8;
    int rank = Board::BOARD_SIZE - 1 - (index / 8);
    return {rank, file};
}

void GameClient::onGameStateChanged(const GameState &gameState)
{
    cachedCurrentPlayer_ = gameState.currentPlayer;
    cachedGameStatus_ = gameState.gameStatus;
    cachedBoard_ = gameState.board;
}

void GameClient::onGameWon(Color winner)
{
    winner_ = winner;
}

void GameClient::onGameDrawn(DrawCause drawCause)
{
    drawCause_ = drawCause;
}

void GameClient::onPendingPromotionChanged(bool pendingPromotion)
{
    cachedPendingPromotion_ = pendingPromotion;
}

void GameClient::promotePawn(PieceType pieceType)
{
    mediator_->onPromotionRequested(static_cast<PieceType>(pieceType));
}

void GameClient::startGame()
{
    mediator_->onStartRequested();
}

void GameClient::restartGame()
{
    mediator_->onRestartRequested();
}

bool GameClient::isGameOngoing() const
{
    return ONGOING_GAME_STATUSES_MASK & (1 << static_cast<int>(cachedGameStatus_));
}