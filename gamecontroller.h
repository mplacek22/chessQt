#pragma once
// #include <PieceTypeQML.h>
#include <QObject>
#include <memory>
#include "game.h"
#include "boardmodel.h"
#include "chessEnums.h"

class GameController : public QObject {
    Q_OBJECT
    Q_PROPERTY(BoardModel* board READ board CONSTANT) // QML expects raw pointer for qt property
    Q_PROPERTY(Chess::Enums::Color currentPlayer READ currentPlayer  NOTIFY currentPlayerChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    Q_PROPERTY(QStringList movesList READ movesList NOTIFY movesChanged)
    Q_PROPERTY(bool pendingPromotion READ pendingPromotion NOTIFY pendingPromotionChanged)

public:
    explicit GameController(QObject* parent = nullptr);

    BoardModel* board() { return boardModel_.get(); }
    Chess::Enums::Color currentPlayer() const;
    QString status() const;
    QStringList movesList() const;
    bool pendingPromotion() const { return game_->pendingPromotion(); }

    Q_INVOKABLE void startGame();
    Q_INVOKABLE void restartGame();
    Q_INVOKABLE void promotePawn(Chess::Enums::PieceType pieceType);

signals:
    void currentPlayerChanged();
    void statusChanged();
    void gameOver(QString winner);
    void movesChanged();
    void pendingPromotionChanged();

private:
    std::shared_ptr<Game> game_;
    std::unique_ptr<BoardModel> boardModel_;

    void onMoveExecuted();
};