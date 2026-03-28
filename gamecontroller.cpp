#include "gamecontroller.h"

GameController::GameController(QObject* parent)
    : QObject(parent),
    game_(std::make_shared<Game>()),
    boardModel_(std::make_unique<BoardModel>(game_))
{
    connect(boardModel_.get(), &BoardModel::moveExecuted,
            this,        &GameController::onMoveExecuted);
}

void GameController::startGame() {
    game_->start();
    emit statusChanged();
    emit currentPlayerChanged();
    boardModel_->refreshAll();
}

void GameController::restartGame() {
    game_->restart();
    emit statusChanged();
    emit currentPlayerChanged();
    boardModel_->refreshAll();
}

QString GameController::currentPlayer() const {
    return game_->currentPlayer() == Color::WHITE ? "white" : "black";
}

QString GameController::status() const {
    switch (game_->status()) {
        case GameStatus::NEW:         return "new";
        case GameStatus::IN_PROGRESS: return "in_progress";
        case GameStatus::DRAW:        return "draw";
        case GameStatus::WHITE_WIN:   return "white_win";
        case GameStatus::BLACK_WIN:   return "black_win";
        default:                      return "unknown";
    }
}

void GameController::onMoveExecuted() {
    emit currentPlayerChanged();

    GameStatus s = game_->status();
    if (s == GameStatus::WHITE_WIN) {
        emit statusChanged();
        emit gameOver("White");
    } else if (s == GameStatus::BLACK_WIN) {
        emit statusChanged();
        emit gameOver("Black");
    } else if (s == GameStatus::DRAW) {
        emit statusChanged();
        emit gameOver("Nobody — it's a draw");
    }
}