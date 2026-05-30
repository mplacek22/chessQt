#include "gamecontroller.h"
#include "pieceUtils.h"
#include "san.h"

GameController::GameController(QObject* parent): QObject(parent),
    game_(std::make_unique<Game>()),
    boardModel_(std::make_unique<BoardModel>(this, this)) {}

BoardModel *GameController::board() const { return boardModel_.get(); }

void GameController::startGame() {
    game_->start();
    emit statusChanged();
    notifyBoardModel();
}

void GameController::restartGame() {
    game_->restart();
    clearSelection();
    clearMoves();
    emit statusChanged();
    emit currentPlayerChanged();
}

void GameController::promotePawn(Chess::Enums::PieceType pieceType)
{
    game_->promotePawn(static_cast<PieceType>(pieceType));
    clearSelection();
    appendMove();
    emit pendingPromotionChanged();
    emit currentPlayerChanged();
}

void GameController::selectSquare(int index)
{
    // can't move if there is pending promotion
    if(game_->pendingPromotion()) return;

    if(selectedSquare_ == -1)
    {
        selectSourceSquare(index);
        return;
    }

    // Same square clicked —> clear selection
    if (selectedSquare_ == index) {
        clearSelection();
        return;
    }

    selectDestinationSquare(index);
}

int GameController::selectedSquare() const
{
    return selectedSquare_;
}

QList<QPair<int, int> > GameController::highlightedSquares() const
{
    return highlightedSquares_;
}

QString GameController::svgPathForSquare(int sq) const
{
    auto coord = indexToCoordinate(sq);
    auto piece = game_->board().getPieceAt(coord);
    return piece ? PieceUtils::imageSource(piece->color(), piece->type()) : "";
}

Chess::Enums::Color GameController::currentPlayer() const {
    return game_->currentPlayer() == Color::WHITE ? Chess::Enums::Color::WHITE : Chess::Enums::Color::BLACK;
}

QString GameController::status() const {
    return PieceUtils::gameStatusToString(game_->status());
}

bool GameController::isGameOngoing() const {
    return game_->isGameOngoing();
}

void GameController::onMoveExecuted() {
    if (game_->pendingPromotion()){
        emit pendingPromotionChanged();
    }
    else {
        clearSelection();
        emit currentPlayerChanged();
        emit statusChanged();
        appendMove();
        notifyBoardModel();
    }
}

void GameController::notifyBoardModel()
{
    emit boardModel_->dataChanged(
        boardModel_->index(0),
        boardModel_->index(63),
        { BoardModel::IsSelectedRole, BoardModel::IsHighlightedRole, BoardModel::SvgPathRole }
        );
}

void GameController::appendMove()
{
    const auto& history = game_->movesHistory();
    const std::size_t total = history.size();
    if (total == 0) return;

    auto lastMove = history.back();
    const QString san = QString::fromStdString(san::toSAN(history.back()));
    if (lastMove.player == Color::WHITE){
        QVariantMap pair;
        pair["white"] = san;
        pair["black"] = "";
        movesList_.append(pair);
    }
    else {
       movesList_.last()["black"] = san;
    }
    emit movesChanged();
}

void GameController::clearMoves()
{
    movesList_.clear();
    emit movesChanged();
}

void GameController::selectSourceSquare(int index)
{
    auto coordinate = indexToCoordinate(index);
    auto piece = game_->board().getPieceAt(coordinate);
    if (!piece || piece->color() != game_->currentPlayer()) return;

    selectedSquare_ = index;

    activeMoves_ = game_->calculatePossibleMovesFromCoord(coordinate);
    highlightedSquares_.clear();
    for (const auto& move : activeMoves_)
        highlightedSquares_.append({move.destination.rank, move.destination.file});

    notifyBoardModel();
}

// Second square clicked — attempt the move
void GameController::selectDestinationSquare(int index)
{
    auto coordinate = indexToCoordinate(index);
    auto it = std::find_if(activeMoves_.begin(), activeMoves_.end(),
                           [&](const Move& m) {
                               return m.destination == coordinate;
                           });
    if (it == activeMoves_.end()) return;

    game_->processMove(*it);
    onMoveExecuted();
}

void GameController::clearSelection()
{
    selectedSquare_ = -1;
    highlightedSquares_.clear();
    activeMoves_.clear();
}

MovePairList GameController::movesList() const {
    return movesList_;
}

bool GameController::pendingPromotion() const { return game_->pendingPromotion(); }

int GameController::winner() const
{
    auto winnerOpt = game_->winner();
    if (winnerOpt.has_value()) {
        return static_cast<int>(winnerOpt.value());
    }
    return -1;
}

QString GameController::drawCause() const
{
    auto drawCauseOpt = game_->drawCause();
    if (drawCauseOpt.has_value()) {
        return PieceUtils::drawCauseToString(drawCauseOpt.value());
    }
    return "";
}