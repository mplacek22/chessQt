#include "boardmodel.h"

BoardModel::BoardModel(Game* game, QObject* parent)
    : QAbstractListModel(parent), game_(game) {}

int BoardModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) return 0;
    return Board::BOARD_SIZE * Board::BOARD_SIZE; // 64 squares
}

QVariant BoardModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= 64) return {};

    const int gridRow = index.row() / Board::BOARD_SIZE;
    const int col     = index.row() % Board::BOARD_SIZE;

    // Flip: visual top (gridRow 0) → board rank 7 (black's back rank)
    const int rank = Board::BOARD_SIZE - 1 - gridRow;

    const Coordinate coord(rank, col);
    const auto piece = game_->board().getPieceAt(coord);

    switch (role) {
    case PieceTypeRole:
        return piece ? pieceTypeToString(piece->type()) : QString("");
    case PieceColorRole:
        return piece ? colorToString(piece->color()) : QString("");
    case IsSelectedRole:
        return (selectedRow_ == rank && selectedCol_ == col);
    case IsHighlightedRole:
        return isHighlighted(rank, col);
    default:
        return {};
    }
}

QHash<int, QByteArray> BoardModel::roleNames() const {
    return {
            { PieceTypeRole,    "pieceType"     },
            { PieceColorRole,   "pieceColor"    },
            { IsSelectedRole,   "isSelected"    },
            { IsHighlightedRole,"isHighlighted" },
            };
}

void BoardModel::selectSquare(int gridRow, int col) {
    // Nothing selected yet — select this square if it has a piece
    const int rank = Board::BOARD_SIZE - 1 - gridRow;
    if (selectedRow_ == -1) {
        Coordinate coord(rank, col);
        auto piece = game_->board().getPieceAt(coord);
        if (!piece) return;

        selectedRow_ = rank;
        selectedCol_ = col;
        emit selectionChanged();
        emit dataChanged(index(0), index(63));
        return;
    }

    // Same square clicked — deselect
    if (selectedRow_ == rank && selectedCol_ == col) {
        selectedRow_ = -1;
        selectedCol_ = -1;
        highlightedSquares_.clear();
        emit selectionChanged();
        emit dataChanged(index(0), index(63));
        return;
    }

    // Second square clicked — attempt the move
    Coordinate source(selectedRow_, selectedCol_);
    Coordinate destination(rank, col);

    game_->board().movePiece(source, destination);
    bool moved = true;

    selectedRow_ = -1;
    selectedCol_ = -1;
    highlightedSquares_.clear();

    if (moved) {
        emit moveExecuted();
        emit boardChanged();
    }

    emit selectionChanged();
    emit dataChanged(index(0), index(63));
}

void BoardModel::refreshAll() {
    emit dataChanged(index(0), index(63));
}

bool BoardModel::isHighlighted(int row, int col) const {
    return highlightedSquares_.contains({row, col});
}

QString BoardModel::pieceTypeToString(PieceType type) const {
    switch (type) {
    case PieceType::KING:   return "King";
    case PieceType::QUEEN:  return "Queen";
    case PieceType::ROOK:   return "Rook";
    case PieceType::BISHOP: return "Bishop";
    case PieceType::KNIGHT: return "Knight";
    case PieceType::PAWN:   return "Pawn";
    default:                return "";
    }
}

QString BoardModel::colorToString(Color color) const {
    return color == Color::WHITE ? "white" : "black";
}