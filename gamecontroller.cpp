#include "gamecontroller.h"
#include "pieceUtils.h"

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
    return PieceUtils::gameStatusToString(game_->status());
}

void GameController::onMoveExecuted() {
    if (game_->pendingPromotion()){
        emit pendingPromotionChanged();
    }
    else {
        emit currentPlayerChanged();
        emit movesChanged();
        emit statusChanged();
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

int GameController::winner() const
{
    auto winnerOpt = game_->winner();
    if (winnerOpt.has_value()) {
        return static_cast<int>(winnerOpt.value());
    }
    return -1;
}

QString GameController::drawCause() const
{
    auto drawCauseOpt = game_->drawCause();
    if (drawCauseOpt.has_value()) {
        return PieceUtils::drawCauseToString(drawCauseOpt.value());
    }
    return "";
}
