#include "boardmodel.h"

#include "pieceUtils.h"

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
            return PieceUtils::imageSource(piece->color(), piece->type());
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
    // cant move if there is a pending promotion
    if (game_->pendingPromotion()) return;

    const int rank = Board::BOARD_SIZE - 1 - gridRow;
    const int file = gridCol;
    if (selectedRow_ == -1) {
        selectSourceSquare(rank, file);
        return;
    }

    // Same square clicked — deselect
    if (selectedRow_ == rank && selectedCol_ == file) {
        deselect();
    }

    selectDestinationSquare(rank, file);
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

void BoardModel::selectSourceSquare(int rank, int file)
{
    Coordinate coord(rank, file);
    auto piece = game_->board().getPieceAt(coord);
    if (!piece) return;
    if (piece->color() != game_->currentPlayer()) return;

    selectedRow_ = rank;
    selectedCol_ = file;


    activeMoves_ = piece->calculatePossibleMoves(
        game_->board().board(),
        {rank, file},
        game_->movesHistory().empty() ? std::nullopt : std::optional(game_->movesHistory().back())
    );
    highlightedSquares_.clear();
    for (const auto& move : activeMoves_)
        highlightedSquares_.append({move->destination.rank(), move->destination.file()});

    emit selectionChanged();
    emit dataChanged(index(0), index(63));
}

void BoardModel::deselect()
{
    selectedRow_ = -1;
    selectedCol_ = -1;
    highlightedSquares_.clear();
    emit selectionChanged();
    emit dataChanged(index(0), index(63));
}

void BoardModel::selectDestinationSquare(int rank, int file)
{
    // Second square clicked — attempt the move
    Coordinate destination(rank, file);
    auto it = std::find_if(activeMoves_.begin(), activeMoves_.end(),
        [&](const std::shared_ptr<Move>& m) {
       return m->destination == destination;
    });

    if (it != activeMoves_.end()) {
        game_->processMove(**it);
        selectedRow_ = -1;
        selectedCol_ = -1;
        highlightedSquares_.clear();
        emit moveExecuted();
        emit boardChanged();
        emit selectionChanged();
        emit dataChanged(index(0), index(63));
    }
}