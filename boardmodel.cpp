#include "boardmodel.h"
#include "gamecontroller.h"

BoardModel::BoardModel(GameController* gameController, QObject* parent)
    : QAbstractListModel(parent), gameController_(std::move(gameController)) {}

int BoardModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) return 0;
    return Board::BOARD_SIZE * Board::BOARD_SIZE; // 64 squares
}

QVariant BoardModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= 64) return {};
    const int gridRow = index.row() / Board::BOARD_SIZE;
    const int file     = index.row() % Board::BOARD_SIZE;

    // Flip: visual top (gridRow 0) → board rank 7 (black's back rank)
    const int rank = Board::BOARD_SIZE - 1 - gridRow; //flipping

    const int square = index.row();
    switch (role) {
    case IsSelectedRole:
        return gameController_->selectedSquare() == square;
    case IsHighlightedRole:
        return gameController_->highlightedSquares().contains({rank, file});
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