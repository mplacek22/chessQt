#include "gamecontroller.h"
#include "pieceUtils.h"
#include "san.h"

GameController::GameController(QObject* parent): QObject(parent),
    boardModel_(std::make_unique<BoardModel>(this, this)) {}

BoardModel *GameController::board() const { return boardModel_.get(); }

void GameController::startGame() {
    GameClient::startGame();
    emit statusChanged();
    notifyBoardModel();
}

void GameController::restartGame() {
    GameClient::restartGame();
    clearSelection();
    clearMoves();
    emit statusChanged();
    emit currentPlayerChanged();
}

void GameController::promotePawn(Chess::Enums::PieceType pieceType)
{
    GameClient::promotePawn(static_cast<PieceType>(pieceType));
    clearSelection();

    emit pendingPromotionChanged();
    emit currentPlayerChanged();
}

void GameController::selectSquare(int index)
{
    // can't move if there is pending promotion
    if(cachedPendingPromotion_) return;

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
    auto piece = cachedBoard_.at(indexToCoordinate(sq));
    return piece ? PieceUtils::imageSource(piece->color, piece->type) : "";
}

void GameController::possibleMovesCalculated(std::vector<Move> moves)
{
    GameClient::possibleMovesCalculated(moves);
    highlightedSquares_.clear();
    for (const auto& move : moves)
        highlightedSquares_.append({move.destination.rank, move.destination.file});
    notifyBoardModel();
}

void GameController::onGameStateChanged(const GameState &gameState)
{
    if (cachedPendingPromotion_) return;
    GameClient::onGameStateChanged(gameState);
    clearSelection();
    if(gameState.lastMove && !cachedPendingPromotion_) {
        appendMove(gameState.lastMove.value());
    }
    emit currentPlayerChanged();
    emit statusChanged();
    notifyBoardModel();
}

void GameController::onGameWon(Color winner)
{
    GameClient::onGameWon(winner);
    emit statusChanged();
}

void GameController::onGameDrawn(DrawCause drawCause)
{
    GameClient::onGameDrawn(drawCause);
    emit statusChanged();
}

Chess::Enums::Color GameController::currentPlayer() const {
    return cachedCurrentPlayer_ == Color::WHITE ? Chess::Enums::Color::WHITE : Chess::Enums::Color::BLACK;
}

QString GameController::status() const {
    return PieceUtils::gameStatusToString(cachedGameStatus_);
}

void GameController::notifyBoardModel()
{
    emit boardModel_->dataChanged(
        boardModel_->index(0),
        boardModel_->index(63),
        { BoardModel::IsSelectedRole, BoardModel::IsHighlightedRole, BoardModel::SvgPathRole }
        );
}

void GameController::appendMove(const Move& move)
{
    const QString san = QString::fromStdString(san::toSAN(move));
    if (move.player == Color::WHITE){
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

Coordinate GameController::indexToCoordinate(int index)
{
    int file = index % 8;
    int rank = Board::BOARD_SIZE - 1 - (index / 8);
    return {rank, file};
}

void GameController::clearMoves()
{
    movesList_.clear();
    emit movesChanged();
}

void GameController::selectSourceSquare(int index)
{
    auto coordinate = indexToCoordinate(index);
    auto piece = cachedBoard_.at(coordinate);
    if (!piece || piece->color != cachedCurrentPlayer_) return;

    selectedSquare_ = index;
    emit squareSelectionChanged();
    mediator_->onPossibleMovesRequested(coordinate);
}

// Second square clicked — attempt the move
void GameController::selectDestinationSquare(int index)
{
    auto coordinate = indexToCoordinate(index);
    auto it = std::find_if(cachedPossibleMoves_.begin(), cachedPossibleMoves_.end(),
                           [&](const Move& m) {
                               return m.destination == coordinate;
                           });
    if (it == cachedPossibleMoves_.end()) return;
    
    mediator_->onMoveRequested(*it);
}

void GameController::clearSelection()
{
    selectedSquare_ = -1;
    emit squareSelectionChanged();
    highlightedSquares_.clear();
    notifyBoardModel();
}

MovePairList GameController::movesList() const {
    return movesList_;
}

bool GameController::pendingPromotion() const {
    return cachedPendingPromotion_;
}

int GameController::winner() const
{
    if (winner_.has_value()) {
        return static_cast<int>(winner_.value());
    }
    return -1;
}

QString GameController::drawCause() const
{
    if (drawCause_.has_value()) {
        return PieceUtils::drawCauseToString(drawCause_.value());
    }
    return "";
}

void GameController::onPendingPromotionChanged(bool pendingPromotion)
{
    GameClient::onPendingPromotionChanged(pendingPromotion);
    emit pendingPromotionChanged();
}