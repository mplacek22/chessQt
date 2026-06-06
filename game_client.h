#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H

#include "Color.h"
#include "DrawCause.h"
#include "Move.h"
#include "colleague.h"

#include <vector>
class GameClient : public Colleague
{
public:
    GameClient() = default;

    virtual void onGameWon(Color winner);
    virtual void onGameDrawn(DrawCause drawCause);
    virtual void onPendingPromotionChanged(bool pendingPromotion);
    virtual void onGameStateChanged(const GameState& gameState);
    virtual void possibleMovesCalculated(std::vector<Move> moves);
    void promotePawn(PieceType pieceType);
    void startGame();
    void restartGame();

protected:
    bool cachedPendingPromotion_ = false;
    bool isGameOngoing_ = true;
    Color cachedCurrentPlayer_ = Color::WHITE;
    GameStatus cachedGameStatus_ = GameStatus::NEW;
    std::vector<Move> cachedPossibleMoves_;
    std::optional<Color> winner_;
    std::optional<DrawCause> drawCause_;
    Board cachedBoard_;
};

#endif // GAME_CLIENT_H
