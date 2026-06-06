#include "boardmodel.h"
#include "gamecontroller.h"

BoardModel::BoardModel(GameController* gameController, QObject* parent)
    : QAbstractListModel(parent), gameController_(std::move(gameController)) {}

int BoardModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) return 0;
    return Board::CELLS_COUNT;
}

QVariant BoardModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= 64) return {};

    const int square = index.row();

    switch (role) {
    case IsSelectedRole:
        return gameController_->selectedSquare() == square;
    case IsHighlightedRole:
        return gameController_->highlightedSquares().contains(square);
    case SvgPathRole:
        return gameController_->svgPathForSquare(square);
    default:
        return {};
    }
}

QHash<int, QByteArray> BoardModel::roleNames() const {
    return {
        { IsSelectedRole, "isSelected"},
        { IsHighlightedRole,"isHighlighted"},
        { SvgPathRole, "svgPath"}
    };
}