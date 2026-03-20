#pragma once
#include <QObject>

class ChessModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool whiteTurn READ whiteTurn NOTIFY whiteTurnChanged)

public:
    explicit ChessModel(QObject *parent = nullptr);

    bool whiteTurn() const;

    Q_INVOKABLE int col(int piece);
    Q_INVOKABLE int row(int piece);
    Q_INVOKABLE bool canDrop(int col, int row);

    Q_INVOKABLE void setActivePiece(int piece);
    Q_INVOKABLE void release(int piece, int col, int row);

signals:
    void whiteTurnChanged();
    void validMove(int col, int row);
    void invalidMove();

private:
    bool m_whiteTurn = true;
};