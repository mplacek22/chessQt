#pragma once

#include <QAbstractListModel>

class GameController;

class BoardModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles {
        IsHighlightedRole = Qt::UserRole + 1,
        IsSelectedRole,
        SvgPathRole
    };

    explicit BoardModel(GameController* controller, QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    GameController* gameController_;
};