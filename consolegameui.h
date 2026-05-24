#pragma once

#include "Game.h"


class ConsoleGameUI : public IGameObserver
{
public:
    explicit ConsoleGameUI(Game& game) : game_(game) {}

    void run();

private:
    Game& game_;

    void displayBoard() const;
    char pieceChar(PieceType type, Color color) const;
    void displayCurrentPlayer() const;
    std::optional<std::string> readSAN() const;
    void gameOver() const;
    std::string drawCauseToString(DrawCause drawCause) const;
    std::string gameStatusToString(GameStatus gameStatus) const;
};
