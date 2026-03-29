#pragma once
#include <QObject>
#include <memory>
#include "game.h"
#include "boardmodel.h"

class GameController : public QObject {
    Q_OBJECT
    Q_PROPERTY(BoardModel* board     READ board          CONSTANT) // QML expects raw pointer for qt property
    Q_PROPERTY(QString currentPlayer READ currentPlayer  NOTIFY currentPlayerChanged)
    Q_PROPERTY(QString status        READ status         NOTIFY statusChanged)
    Q_PROPERTY(QStringList movesList READ movesList      NOTIFY movesChanged)

public:
    explicit GameController(QObject* parent = nullptr);

    BoardModel* board() { return boardModel_.get(); }
    QString currentPlayer() const;
    QString status() const;

    Q_INVOKABLE void startGame();
    Q_INVOKABLE void restartGame();
    QStringList movesList() const;

signals:
    void currentPlayerChanged();
    void statusChanged();
    void gameOver(QString winner);
    void movesChanged();

private:
    std::shared_ptr<Game> game_;
    std::unique_ptr<BoardModel> boardModel_;

    void onMoveExecuted();
};