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
    for (int i = 0; i < 64; ++i) {
        auto piece = gameState.board.at(indexToCoordinate(i));
        cachedPieces_[i] = piece
                               ? std::make_optional(std::pair{piece->color(), piece->type()})
                               : std::nullopt;
    }
    cachedCurrentPlayer_ = gameState.currentPlayer;
    cachedGameStatus_ = gameState.gameStatus;
}

void GameClient::onGameWon(Color winner)
{
    winner_ = winner;
    isGameOngoing_ = false;
}

void GameClient::onGameDrawn(DrawCause drawCause)
{
    drawCause_ = drawCause;
    isGameOngoing_ = false;
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