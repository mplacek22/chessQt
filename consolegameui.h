#pragma once

#include "game_client.h"



class ConsoleGameUI : public GameClient
{
public:
    void run();

    void possibleMovesCalculated(std::vector<Move> moves) override;
    void onGameStateChanged(const GameState& gameState) override;
    void onGameWon(Color winner) override;
    void onGameDrawn(DrawCause drawCause) override;
    void onPendingPromotionChanged(bool pendingPromotion) override;

private:
    void displayBoard() const;
    char pieceChar(PieceType type, Color color) const;
    void displayCurrentPlayer() const;
    void displayStatus() const;
    [[nodiscard]] std::optional<Coordinate> promptCoordinate(const std::string& prompt) const;

    [[nodiscard]] std::string drawCauseToString(DrawCause drawCause) const;
    [[nodiscard]] std::string gameStatusToString(GameStatus gameStatus) const;

    static constexpr std::array<PieceType, 4> PROMOTION_PIECES = {PieceType::QUEEN, PieceType::KNIGHT, PieceType::BISHOP, PieceType::ROOK};
};
