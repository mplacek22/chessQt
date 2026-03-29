#include "boardmodel.h"

BoardModel::BoardModel(std::shared_ptr<Game> game, QObject* parent)
    : QAbstractListModel(parent), game_(std::move(game)) {}

int BoardModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) return 0;
    return Board::BOARD_SIZE * Board::BOARD_SIZE; // 64 squares
}

QVariant BoardModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= 64) return {};

    const int gridRow = index.row() / Board::BOARD_SIZE;
    const int file     = index.row() % Board::BOARD_SIZE;

    // Flip: visual top (gridRow 0) → board rank 7 (black's back rank)
    const int rank = Board::BOARD_SIZE - 1 - gridRow; //flipping

    const Coordinate coord(rank, file);
    const auto piece = game_->board().getPieceAt(coord);

    switch (role) {
    case IsSelectedRole:
        return (selectedRow_ == rank && selectedCol_ == file);
    case IsHighlightedRole:
        return isHighlighted(rank, file);
    case SvgPathRole: {
            if (!piece) return QString("");
            return QString("pieces/%1_%2.svg")
                .arg(colorToString(piece->color()))
                .arg(pieceTypeToString(piece->type()));
        }
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

void BoardModel::selectSquare(int gridRow, int gridCol) {
    // Nothing selected yet — select this square if it has a piece
    const int rank = Board::BOARD_SIZE - 1 - gridRow;
    const int file = gridCol;
    if (selectedRow_ == -1) {
        Coordinate coord(rank, file);
        auto piece = game_->board().getPieceAt(coord);
        if (!piece) return;
        if (piece->color() != game_->currentPlayer()) return;

        selectedRow_ = rank;
        selectedCol_ = file;
        emit selectionChanged();
        emit dataChanged(index(0), index(63));
        return;
    }

    // Same square clicked — deselect
    if (selectedRow_ == rank && selectedCol_ == file) {
        selectedRow_ = -1;
        selectedCol_ = -1;
        highlightedSquares_.clear();
        emit selectionChanged();
        emit dataChanged(index(0), index(63));
        return;
    }

    // Second square clicked — attempt the move
    Coordinate source(selectedRow_, selectedCol_);
    Coordinate destination(rank, file);
    Move move(source, destination, game_->currentPlayer());


    game_->processMove(move);
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
    case PieceType::KING:   return "king";
    case PieceType::QUEEN:  return "queen";
    case PieceType::ROOK:   return "rook";
    case PieceType::BISHOP: return "bishop";
    case PieceType::KNIGHT: return "knight";
    case PieceType::PAWN:   return "pawn";
    default:                return "";
    }
}

QString BoardModel::colorToString(Color color) const {
    return color == Color::WHITE ? "w" : "b";
}