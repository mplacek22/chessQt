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
    emit isGameOngoingChanged();
    emit currentPlayerChanged();
    boardModel_->refreshAll();
}

void GameController::restartGame() {
    game_->restart();
    emit statusChanged();
    emit currentPlayerChanged();
    emit movesChanged();
    boardModel_->refreshAll();
}

void GameController::promotePawn(Chess::Enums::PieceType pieceType)
{
    game_->promotePawn(static_cast<PieceType>(pieceType));
    emit pendingPromotionChanged();
    emit movesChanged();
    emit currentPlayerChanged();

    boardModel_->refreshAll();
}

Chess::Enums::Color GameController::currentPlayer() const {
    return game_->currentPlayer() == Color::WHITE ? Chess::Enums::Color::WHITE : Chess::Enums::Color::BLACK;
}

QString GameController::status() const {
    switch (game_->status()) {
        case GameStatus::NEW:         return "new";
        case GameStatus::IN_PROGRESS: return "in_progress";
        case GameStatus::DRAW:        return "draw";
        case GameStatus::WHITE_WIN:   return "white_win";
        case GameStatus::BLACK_WIN:   return "black_win";
        case GameStatus::SINGLE_CHECK: return "single_check";
        case GameStatus::DOUBLE_CHECK: return "single_check";
        default:                      return "unknown";
    }
}

void GameController::onMoveExecuted() {
    if (game_->pendingPromotion()){
        emit pendingPromotionChanged();
    }
    else {
        emit currentPlayerChanged();
        emit movesChanged();
        emit isGameOngoingChanged();
    }

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

QStringList GameController::movesList() const {
    QStringList result;
    const auto& moves = game_->movesHistory();
    for (size_t i = 0; i < moves.size(); i += 2) {
        QString entry = QString("%1. %2").arg(i/2 + 1)
        .arg(QString::fromStdString(Move::toChessNotation(moves[i])));
        if (i + 1 < moves.size())
            entry += "  " + QString::fromStdString(Move::toChessNotation(moves[i+1]));
        result.append(entry);
    }
    return result;
}

bool GameController::isGameOngoing() const
{
    return ONGOING_GAME_STATUSES_MASK & (1 << static_cast<int>(game_->status()));
}
