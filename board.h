#pragma once
#include <QAbstractListModel>
#include <QVector>
#include <QString>

class ChessBoard : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(QString fen READ fen WRITE setFen NOTIFY fenChanged)
    Q_PROPERTY(int selectedIndex READ selectedIndex NOTIFY selectedIndexChanged)

public:
    enum Roles { PieceRole = Qt::UserRole + 1, FileRole, RankRole };

    explicit ChessBoard(QObject* parent = nullptr);

    // QAbstractListModel
    int rowCount(const QModelIndex& = {}) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    // Properties
    QString fen() const;
    void setFen(const QString& fen);
    int selectedIndex() const { return m_selectedIndex; }

    Q_INVOKABLE void selectSquare(int file, int rank);
    Q_INVOKABLE QString pieceAt(int file, int rank) const;

signals:
    void fenChanged();
    void selectedIndexChanged();
    void moveMade(int fromFile, int fromRank, int toFile, int toRank);

private:
    // board[rank * 8 + file], rank 0 = rank 1, rank 7 = rank 8
    QVector<QString> m_board; // "wK", "bQ", "" etc.
    int m_selectedIndex = -1;

    void loadFen(const QString& fen);
    QString buildFen() const;
    bool isValidMove(int fromFile, int fromRank, int toFile, int toRank) const;
    bool isWhitePiece(const QString& p) const { return !p.isEmpty() && p[0] == 'w'; }
    bool isBlackPiece(const QString& p) const { return !p.isEmpty() && p[0] == 'b'; }
    int toIndex(int file, int rank) const { return rank * 8 + file; }

    QString m_fen;
    bool m_whiteTurn = true;
};