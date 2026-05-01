#pragma once
#include <QAbstractListModel>
#include "game.h"

class BoardModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int selectedRow READ selectedRow NOTIFY selectionChanged)
    Q_PROPERTY(int selectedCol READ selectedCol NOTIFY selectionChanged)

public:
    enum Roles {
        IsHighlightedRole,
        IsSelectedRole,
        SvgPathRole
    };

    explicit BoardModel(std::shared_ptr<Game> game, QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    int selectedRow() const { return selectedRow_; }
    int selectedCol() const { return selectedCol_; }

    Q_INVOKABLE void selectSquare(int row, int col);
    void refreshAll();

signals:
    void selectionChanged();
    void boardChanged();
    void moveExecuted();

private:
    std::shared_ptr<Game> game_;
    int selectedRow_ = -1;
    int selectedCol_ = -1;
    QList<QPair<int,int>> highlightedSquares_;
    std::vector<std::shared_ptr<Move>> activeMoves_;

    bool isHighlighted(int row, int col) const;
    void selectSourceSquare(int rank, int file);
    void deselect();
    void selectDestinationSquare(int rank, int file);


};